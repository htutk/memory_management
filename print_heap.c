// Tyler Ho, Alex Htut
// cse374-19sp-students-htutk-tmhh
// CSE 374 | HW 6
// 05/21/2019
// print_heap.c
// Implementation of function print_heap.

// Preprocessor inclusions
#include "mem.h"
#include "mem_impl.h"

// print a formatted listing on file f,
// showing the blocks on the free list.
// Each line: Block #: address, size (both in hexadecimals).
// assumes f is valid and opened in write-mode.
void print_heap(FILE * f) {
  int count = 0;
  freeListNode* current = freeList;
  while (current != NULL) {
    count++;
    fprintf(f, "Block %d: addr=%p size=%ld\n", \
            count, (void *) current->addr, \
            current->size);
    current = current->next;
  }
  fprintf(f, "\n");
}
