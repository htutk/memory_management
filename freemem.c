// Tyler Ho, Alex Htut
// cse374-19sp-students-htutk-tmhh
// CSE 374 | HW 6
// 05/21/2019
// freemem.c
// Implementation of freemem.

// Preprocessor Inclusions
// Included mem.h to use OFFSET definition
#include "mem_impl.h"
#include "mem.h"

// Function Declarations
void insertFreeNode(freeListNode* my_freeList,
                    size_t free_size,
                    uintptr_t free_addr);
int isAdjacent(uintptr_t addr1, uintptr_t addr2, size_t size1, size_t size2);
void combineAdjacents(freeListNode* my_freeList);

// Free the block of storage pointed by pointer p.
// Exits immediately if p is NULL
// Pointer p must be created by getmem() or must not
// be freed previously, otherwise behavior is undefined (May crash).
void freemem(void* p) {
  if (p == NULL) {
    return;
  }
  uintptr_t addr = (uintptr_t) p;
  size_t *sizePtr = (size_t *) (addr - OFFSET);
  size_t size = *sizePtr;

  insertFreeNode(freeList, size, addr);
  check_heap();
}

// Inserts free'd node by user in corresponding
// location preserving ascending addresses
void insertFreeNode(freeListNode* my_freeList,
                    size_t free_size,
                    uintptr_t free_addr) {
  freeListNode* current = my_freeList;
  int added_node = 0;  // Boolean to check if node was added or not

  // Empty free list case
  if (current == NULL) {
    insertFirstNode(free_size, free_addr, NULL);
    return;
  }

  // Add node in front of head case
  if (free_addr < current->addr) {
    insertFirstNode(free_size, free_addr, current);
    combineAdjacents(freeList);
    combineAdjacents(freeList);
    return;
  }

  // Middle case (No combinations yet)
  while (current->next != NULL) {
    // Add node in specified location and keep ascending address order
    if (current->next->addr > free_addr) {
      freeListNode* newNode = makeNewNode(free_size, free_addr, current->next);
      current->next = newNode;
      added_node = 1;
      break;
    }
    current = current->next;
  }

  // End case: Made it through list and did not add yet
  if (!added_node) {
    current->next = makeNewNode(free_size, free_addr, NULL);
  }

  // Clean list by combining adjacent nodes
  combineAdjacents(my_freeList);
  combineAdjacents(my_freeList);
}

// Looks through list and combines nodes from
// left to right if they are adjacent.
void combineAdjacents(freeListNode* my_freeList) {
  freeListNode* current = my_freeList;

  if (current == NULL) {
    fprintf(stderr, "Error: Atempting to combine empty list");
  }

  while (current->next != NULL) {
    if (isAdjacent(current->addr, \
                   current->next->addr, \
                   current->size, \
                   current->next->size)) {
      // Combine nodes
      // address doesn't need to be changed since current will always be less)
      freeListNode* nextNode = current->next;
      current->size = current->size + nextNode->size + OFFSET;
      current->next = nextNode->next;
      free(nextNode);
    } else {
    current = current->next;
    }
  }
}

// determine if the given two nodes are adjacent to each other.
// return 1 if so, 0 if not.
int isAdjacent(uintptr_t addr1, uintptr_t addr2, size_t size1, size_t size2) {
  if (addr1 > addr2) {
    return (addr1 - addr2 - size2) == OFFSET;
  } else {
    return (addr2 - addr1 - size1) == OFFSET;
  }
}
