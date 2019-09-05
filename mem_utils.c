// Tyler Ho, Alex Htut
// cse374-19sp-students-htutk-tmhh
// CSE 374 | HW 6
// 05/21/2019
// mem_utils.c
// Implementation of function check_heap
// This funciton is intended for testing purposes.
// Not to be used in client side.

// Preprocessor Inclusions
#include <assert.h>
#include "mem_impl.h"
#include "mem.h"

// check for possible errors with the heap.
// perform assertion tests on:
// * Blocks are ordered with strictly increasing memory addresses
// * Block sizes are positive numbers and no smaller than whatever
//   minimum size you are using in your implementation
// * Blocks do not overlap (the start + length of a block is not
//   an address in the middle of a later block on the list)
// * Blocks are not touching (the start + length of a block should not
//   be the same address as the next block on the list since in that case
//   the two blocks should have been combined into a single, larger block.)
// raises assertion errors if any assertion fails.
// otherwise finishes quitely.

void check_heap() {
  freeListNode* current = freeList;
  while (current != NULL && current->next != NULL) {
    // Increasing memory addresses
    assert(current->addr < current->next->addr);

    // Positive size and greater than min size
    assert(current->size > 0);

    // Blocks Don't Overlap
    assert((current->addr + current->size) + OFFSET < (current->next->addr));

    // Blocks Don't Touch
    assert((current->addr + current->size) + OFFSET != (current->next->addr));

    current = current->next;
  }
  return;
}
