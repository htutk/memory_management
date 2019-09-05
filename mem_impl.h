// Tyler Ho, Alex Htut
// cse374-19sp-students-htutk-tmhh
// CSE 374 | HW 6
// 05/21/2019
// mem_impl.h
// Header file with declarations of internal implementation details
// shared by more than one of the above files.
// This is information required in more than one of the implementation files,
// but that is not part of the public interface,
// which is declared in file mem.h.
// In particular, this is where the declarations of
// the free list data structure(s)
// should reside, as well as the declaration of function check_heap.

#ifndef MEMI_HEADER
#define MEMI_HEADER

// Preprocessor inclusions
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// new typedef freeListNode to form freeList,
// which keeps track of avaiable memory blocks.
typedef struct freeListNode {
  size_t size;
  uintptr_t addr;
  struct freeListNode* next;
  struct freeListNode* prev;
} freeListNode;

// global variable used across the implementation
extern freeListNode* freeList;
extern uintptr_t total;

void check_heap();

freeListNode* makeNewNode(size_t size, uintptr_t addr, freeListNode* next);

void insertNewNode(freeListNode* freeList, size_t newSize, uintptr_t newAddr);

void insertFirstNode(size_t free_size, uintptr_t free_addr, freeListNode* next);

#endif
