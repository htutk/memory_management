// Tyler Ho, Alex Htut
//
// CSE 374 | HW 6
// 05/21/2019
// bench.c
// Source code for bench program

// Preprocessor Inclusions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mem.h"

// Preprocessor Macros
#define MAX_IN 50

// Function Declarations
int setParamInfo(char* param_name, int* params);
int strToInt(char* string);
void printParams(int* params, int size);
void test_setParamInfo();
void test_strToInt();
void manual_bench_test0();
void manual_bench_test1();
void perform_bench_tests();
void setFE(void *pointer, size_t size);
void unsetFE(void *pointer);

int main(int argc, char **argv) {
  // ntrials, pctget, pctlarge, small_limit, large_limit, random_seed
  int params[6] = {10000, 50, 10, 200, 20000, 0};

  // Set random seed to time of day
  time_t now;
  struct tm *tm;
  now = time(0);
  if ((tm = localtime(&now)) == NULL) {
    printf("Error extracting time stuff\n");
    return 1;
  }
  params[5] = tm->tm_hour * 60 + tm->tm_min;

  // Read parameters:
  for (int i = 1; i < argc; i++) {
    setParamInfo(argv[i], params);
  }

  // Perform bench tests:
  //  test_strToInt();
  //  test_setParamInfo();
  //  manual_bench_test1();
  perform_bench_tests(params);
  exit(EXIT_SUCCESS);
}

// Performs tests using given parameters by user
// Will print to stdio out the CPU time taken,
// total amount of storage taken from underlying system,
// total number of blocks in freelist,
// Will also produce output file giving information on
// blocks
void perform_bench_tests(int* params) {
  int ntrials = params[0];
  int pctget = params[1];
  int pctlarge = params[2];
  int small_limit = params[3];
  int large_limit = params[4];
  int random_seed = params[5];

  void* blocks[ntrials];

  FILE *f = fopen("output.txt", "a");
  int size = 0;

  srand(random_seed);

  // For CPU time
  clock_t start, end;
  double cpu_time_used;
  start = clock();
  float percent = 0.1;

  for (int i = 0; i < ntrials; i++) {
    if ((rand() % 100 + 1) <= pctget) {  // rand num from 1-100
      void* p;
      // getmem
      if ((rand() % 100 + 1) <= pctlarge) {  // rand num from 1-100
        // Large block
        // rand num from small_limit - large_limit
        size_t pSize = (size_t) (rand() % (large_limit - small_limit)\
                                 + small_limit + 1);
        p = getmem(pSize);
        setFE(p, pSize);
      } else {
        // Small block
        // rand num from 1-small_limit
        size_t pSize = (size_t) (rand() % small_limit + 1);
        p = getmem(pSize);
        setFE(p, pSize);
      }
      blocks[size] = p;
      size++;
    } else {
      if (size > 0) {
        int index = rand() % size;
        freemem(blocks[index]);
        unsetFE(blocks[index]);
        blocks[index] = blocks[size - 1];
        size--;
      }
    }

    // Pointers to store mem stats
    uintptr_t* total_size = (uintptr_t*) malloc(sizeof(uintptr_t*));
    uintptr_t* total_free = (uintptr_t*) malloc(sizeof(uintptr_t*));
    uintptr_t* n_free_blocks = (uintptr_t*) malloc(sizeof(uintptr_t*));

    if (((float) i / ntrials >= percent) || i == ntrials - 1) {
      end = clock();
      printf("end is %ld\n", end);
      cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
      get_mem_stats(total_size, total_free, n_free_blocks);
      printf("***--------------%d--------------***\n", \
             (int) (percent * 100));
      printf("CPU Time in seconds: %f\n", \
             cpu_time_used);
      printf("Total amount of storage from underlying system: %ld\n", \
             *total_size);
      printf("Total number of blocks in freelist: %ld\n", \
             *n_free_blocks);
      printf("Average numbers of bytes in free storage: %ld\n", \
             *total_free / *n_free_blocks);

      percent = percent + 0.1;
    }

    // Free pointers used for mem_stats
    free(total_size);
    free(total_free);
    free(n_free_blocks);
  }
  printf("\n");
  fclose(f);
}

// Prints parameter values given by user
void printParams(int* params, int size) {
  for (int i = 0; i < size; i++) {
    printf("%d ", params[i]);
  }
  printf("\n");
}

// Will return number to set parameter to
// Will exit if invalid parameter
int setParamInfo(char* param_string, int* params) {
  // Split string
  int i = 0;
  int e_index = -1;
  // just use strchr here
  while (param_string[i]) {
    if (param_string[i] == '=') {
      e_index = i;
    }
    i++;
  }

  if (e_index == -1 || e_index == strlen(param_string) - 1) {
    fprintf(stderr, "Error: Bad param '%s'\n", param_string);
    exit(EXIT_FAILURE);
  }

  char param_name[MAX_IN];
  char param_strNum[MAX_IN];

  strncpy(param_name, param_string, e_index);
  strncpy(param_strNum, param_string + e_index + 1, strlen(param_name));

  int param_num = strToInt(param_strNum);

  // Set parameters
  // ntrials, pctget, pctlarge, small_limit, large_limit, random_seed
  if (!strncmp(param_name, "ntrials", e_index)) {
    params[0] = param_num;
  } else if (!strncmp(param_name, "pctget", e_index)) {
    params[1] = param_num;
  } else if (!strncmp(param_name, "pctlarge", e_index)) {
    params[2] = param_num;
  } else if (!strncmp(param_name, "small_limit", e_index)) {
    params[3] = param_num;
  } else if (!strncmp(param_name, "large_limit", e_index)) {
    params[4] = param_num;
  } else if (!strncmp(param_name, "random_seed", e_index)) {
    params[5] = param_num;
  } else {
    fprintf(stderr, "Error: Bad param name %s\n", param_name);
    exit(EXIT_FAILURE);
  }

  return 0;
}

// Will return given string to int "321" -> 321
// Will exit if invalid string
int strToInt(char* str) {
  int result = 0;
  int i = 0;
  while (str[i]) {
    // Ensure character is a number
    if (str[i] >= '0' && str[i] <= '9') {
      result = (result * 10) + (str[i]-'0');
      str++;
    } else {
      fprintf(stderr, "Error: Invalid string to int '%s'\n", str);
      exit(EXIT_FAILURE);
    }
  }
  return result;
}

// Testing utility method strToInt
void test_strToInt() {
  printf("Testing string to int: %d\n", strToInt("1"));
  printf("Testing string to int: %d\n", strToInt("100"));
  printf("Testing string to int: %d\n", strToInt("999"));
  printf("Testing string to int: %d\n", strToInt("1234567890"));
  // printf("Testing string to int fail: %d\n", strToInt("-123"));
  // printf("Testing string to int fail: %d\n", strToInt("123a"));
  printf("Testing string to int fail: %d\n", strToInt("hello"));
}

// Testing utility method setParamInfo
void test_setParamInfo() {
  int params[6] = {1000, 50, 10, 200, 20000, 0};
  printf("Before: ");
  printParams(params, 6);
  setParamInfo("ntrials=2000", params);
  setParamInfo("pctget=25", params);
  printf("After: ");
  printParams(params, 6);
}

// Stores hex value 0xFE to first 16 bytes (or smaller)
// to given pointer
void setFE(void *pointer, size_t size) {
  if (size > 16) {
    size = 16;
  }
  unsigned char filler = 0xFE;

  for (int i = 0; i < size; i++) {
    unsigned char *current = (unsigned char *) ((uintptr_t) pointer + i);
    *current = filler;
  }
}

// Deletes hex value 0xFE and replaces it with
// 0x00 to given pointer (for freeing)
void unsetFE(void *pointer) {
  unsigned char filler = 0x00;
  for (int i = 0; i < 16; i++) {
    unsigned char *current = (unsigned char *) ((uintptr_t) pointer + i);
    *current = filler;
  }
}

// Manual test for insertion where
// node is adjacent on both sides
void manual_bench_test0() {
  // Using randseed 3 and ntrials=10 as starter
  FILE *f = fopen("output.txt", "a");

  printf("i: 1 getmemed\n");
  void* p0 = getmem(169);
  print_heap(f);

  printf("i: 2 getmemed\n");
  void* p1 = getmem(141);
  print_heap(f);

  printf("i: 3 freememed\n");
  freemem(p0);
  print_heap(f);

  printf("i: 4 freememed\n");
  freemem(p1);
  print_heap(f);

  fclose(f);
}

// Manual test case for edge case
// get mem twice to get 4k memory block and free both blocks
void manual_bench_test1() {
  FILE *f = fopen("output.txt", "a");

  printf("i: 1 getmemed\n");
  void* p0 = getmem(2048);
  print_heap(f);

  printf("i: 2 getmemed\n");
  void* p1 = getmem(2032);
  print_heap(f);

  printf("i: 3 freememed\n");
  freemem(p0);
  print_heap(f);

  printf("i: 4 freememed\n");
  freemem(p1);
  print_heap(f);

  fclose(f);
}
