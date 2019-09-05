// Tyler Ho, Alex Htut
// CSE 374 | HW 6
// 05/21/2019
// mem.h
// Header file containing declarations of the public functions
// in the memory manager (including appropriate comments).
// This is the interface that clients of your getmem/freemem package would use.

#ifndef MEM_HEADER
#define MEM_HEADER

// Preprocessor Inclusions
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Preprocessor Macros
#define SIZE_T_SIZE 8
#define OFFSET 16

void* getmem(uintptr_t size);

void freemem(void* p);

void get_mem_stats(
  uintptr_t* total_size,
  uintptr_t* total_free,
  uintptr_t* n_free_blocks);

void print_heap(FILE *f);

#endif
