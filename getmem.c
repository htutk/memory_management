// Tyler Ho, Alex Htut
// cse374-19sp-students-htutk-tmhh
// CSE 374 | HW 6
// 05/21/2019
// getmem.c
// Implementation of function getmem.

// Preprocessor inclusions
#include "mem.h"
#include "mem_impl.h"

// Preprocessor macros
#define fourK 4096

// Function declarations
size_t hasSpace(freeListNode* freeList, uintptr_t size);
void* split(freeListNode* freeList, uintptr_t size, size_t space, int added);
void getMore(uintptr_t size);
void freeLastNode(size_t size);

// Global vars initializations
freeListNode* freeList = NULL;
uintptr_t total = 0;

// returns a pointer to a new block
// with at least 'size' bytes of memory.
// the returned address is a multiple of 16.
// returns NULL if size <= 0 or malloc fails.
void* getmem(uintptr_t size) {
  check_heap();
  if (size <= 0) {
    return NULL;
  }
  if (size % 16 != 0) {
    size = size + (16 - size % 16);
  }

  size_t space = hasSpace(freeList, size);
  int added = 0;

  if (space) {
    check_heap();
    return split(freeList, size, space, added);
  } else {
    getMore(size);
    if (freeList == NULL) {
      fprintf(stderr, "No more memory.\n");
      return NULL;
    }
    added = 1;
    space = size;
    check_heap();
    return split(freeList, size, space, added);
  }
  check_heap();
}

// checks if the freeList has a block with size bigger
// than or equal to the size.
// returns the size of the blocks if so; returns 0 if not.
size_t hasSpace(freeListNode* list, uintptr_t size) {
  freeListNode* current = list;
  while (current != NULL) {
    if (current->size >= size) {
      return current->size;
    }
    current = current->next;
  }
  return 0;
}

// freeList: the list that keeps track of all available memory
// size: requested param in getmem()
// space: identifier to help find "the" block in freeList
// Does the following:
//  identies the block with size "space" in freeList,
//  updates the splited block with new information: size, addr (if splited),
//  updates freeList appropriately (removes the block if good size is found,
//  splits and adds the new block while maintaining the ascending order if the
//  block is split), and
//  returns a pointer to the recently freed block
//  (with the size info attached in front).
void* split(freeListNode* freeList1, uintptr_t size, size_t space, int added) {
  freeListNode* current = freeList1;

  if (freeList1->size == space) {
    size_t *firstChunk = (size_t *) (current->addr - OFFSET);
    *firstChunk = (size_t) size;
    uintptr_t firstChunkStart = current->addr;
    uintptr_t firstChunkEnd = firstChunkStart + size;

    size_t newFirstSize = current->size - size - OFFSET;

    if ((intptr_t) newFirstSize > OFFSET * 2) {
      size_t *leftOverFirstChunk = (size_t *) firstChunkEnd;
      *leftOverFirstChunk = newFirstSize;
      uintptr_t newFirstAddr = firstChunkEnd + OFFSET;
      current->size = newFirstSize;
      current->addr = newFirstAddr;
    } else {
      *firstChunk = *firstChunk + newFirstSize + OFFSET;
      freeList = freeList1->next;
      free(current);
    }
    return (void *) firstChunkStart;
  }

  // space identifies the block
  // guarantees that one of the block will have 'space' size.
  while (current->next != NULL && current->next->size != space) {
    current = current->next;
  }

  // if there is only one block in freeList (the beginning case),
  // or the last block has the 'space' size,
  // the split/update freeList procedure is slightly different.
  if (current->next == NULL) {
    // computes the to-be-returned information
    size_t *lastChunk = (size_t *) (current->addr - OFFSET);
    *lastChunk = (size_t) size;
    uintptr_t lastChunkStart = current->addr;
    uintptr_t lastChunkEnd = lastChunkStart + size;

    // computes the new information for the 'new' block
    size_t newLastSize = current->size - size - OFFSET;

    if ((intptr_t) newLastSize > OFFSET * 4) {
      size_t *leftOverLastChunk = (size_t *) lastChunkEnd;
      *leftOverLastChunk = newLastSize;
      uintptr_t newLastAddr = lastChunkEnd + OFFSET;

      // only one node in freeList (beginning case)
      // replace self with new information
      current->size = newLastSize;
      current->addr = newLastAddr;
    } else {
      *lastChunk = *lastChunk + newLastSize + OFFSET;
      freeLastNode(size);
    }
    return (void *) lastChunkStart;
  } else {
    size_t *chunk = (size_t *) (current->next->addr - OFFSET);
    *chunk = (size_t) size;
    uintptr_t chunkStart = current->next->addr;
    uintptr_t chunkEnd = chunkStart + size;

    freeListNode* rest = current->next->next;
    size_t newSize = current->next->size - size - OFFSET;

    if ((intptr_t) newSize > OFFSET * 4) {
      // compute new size and address
      size_t *leftOverChunk = (size_t *) chunkEnd;
      *leftOverChunk = newSize;
      uintptr_t newAddr = chunkEnd + OFFSET;

      current->next->size = newSize;
      current->next->addr = newAddr;
    } else {
      *chunk = *chunk + newSize + OFFSET;
      free(current->next);
      current->next = rest;
    }
    return (void *) chunkStart;
  }
}

// insert a new node with 'newSize' and 'newAddr' while maintaining
// ascending order of address
void insertNewNode(freeListNode* freeList1, size_t newSize, uintptr_t newAddr) {
  if (freeList == NULL) {
    freeList = makeNewNode(newSize, newAddr, NULL);
  }

  if (newAddr < freeList->addr) {
    freeListNode* newNode = makeNewNode(newSize, newAddr, freeList1);
    freeList = newNode;
  } else {
    freeListNode* current = freeList1;
    while (current->next != NULL && newAddr > current->next->addr) {
      current = current->next;
    }
    current->next = makeNewNode(newSize, newAddr, current->next);
  }
}

// requests memory from the underlying system (4K each time).
// intended to be called whenever free list has no avaiable 'size' blocks.
void getMore(uintptr_t size) {
  uintptr_t getSize = fourK;
  if (size > getSize) {
    getSize = size + (size % 16);
  }

  uintptr_t newMem = (uintptr_t) malloc(getSize + OFFSET);
  if ((void *) newMem == NULL) {
    fprintf(stderr, "Error: Malloc fails\n");
    freeList = NULL;
  }
  total = total + getSize + OFFSET;

  size_t newSize = getSize;
  size_t *newSizePtr = (size_t *) newMem;
  *newSizePtr = newSize;
  uintptr_t newAddr = newMem + OFFSET;

  if (freeList == NULL) {  // freeList is empty
    freeList = makeNewNode(newSize, newAddr, NULL);
  } else {
    insertNewNode(freeList, newSize, newAddr);
  }
}

// insert a new node with 'free_size' and 'free_addr' at the very beginning.
// intended to be used in freemem()
void insertFirstNode(size_t f_size, uintptr_t f_addr, freeListNode* next) {
  freeList = makeNewNode(f_size, f_addr, next);
}

// remove the last node in the freeList if its size is 'size'.
void freeLastNode(size_t size) {
  freeListNode* current = freeList;
  while (current != NULL && current->next != NULL) {
    if (current->next->size == size) {
      free(current->next);
      current->next = NULL;
    }
    current = current->next;
  }
}
