// Tyler Ho, Alex Htut
// cse374-19sp-students-htutk-tmhh
// CSE 374 | HW 6
// 05/21/2019
// makeNewNode.c
// makeNewNode method for freeListNode struct

// Preprocessor Inclusions
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// Preprocessor Macros
#include "mem.h"
#include "mem_impl.h"

// Creates a freeListNode struct and returns it
freeListNode* makeNewNode(size_t size, uintptr_t addr, freeListNode* next) {
  freeListNode* newNode = (freeListNode *) malloc(sizeof(freeListNode));
  if (newNode == NULL) {
    printf("newNode is null\n");
    return NULL;
  }
  newNode->size = size;
  newNode->addr = addr;
  newNode->next = next;
  return newNode;
}
