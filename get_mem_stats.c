// Tyler Ho, Alex Htut
// cse374-19sp-students-htutk-tmhh
// CSE 374 | HW 6
// 05/21/2019
// get_mem_stats.c
// Implementation of function get_mem_stats.

// Preprocessor Inclusions
#include "mem.h"
#include "mem_impl.h"

// Function Declarations
uintptr_t countBlocks();
uintptr_t countFreeBytes();

// store the three stats about the current state of the memory.
// intended to be called after each getmem() and freemem().
void get_mem_stats(
  uintptr_t* total_size,
  uintptr_t* total_free,
  uintptr_t* n_free_blocks) {
  *total_size = total;
  *total_free = countFreeBytes();
  *n_free_blocks = countBlocks();
}

// iterates over freeList and sums up the total avaiable size
uintptr_t countFreeBytes() {
  freeListNode* current = freeList;
  size_t count = 0;
  while (current != NULL) {
    count = count + current->size;
    current = current->next;
  }
  return count;
}

// iterates over freeList and counts the number of blocks
uintptr_t countBlocks() {
  freeListNode* current = freeList;
  uintptr_t count = 0;
  while (current != NULL) {
    count++;
    current = current->next;
  }
  return count;
}
