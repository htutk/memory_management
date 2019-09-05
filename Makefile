# Tyler Ho, Alex Htut
# CSE 374 | HW 6
# 05/21/2019
# Makefile
# Makefile for make program

# make the main 'bench' executable
all: bench

bench: bench.o getmem.o freemem.o get_mem_stats.o print_heap.o mem_utils.o makeNewNode.o
	gcc -Wall -std=c11 -g -o bench bench.o getmem.o freemem.o get_mem_stats.o print_heap.o mem_utils.o makeNewNode.o

getmem.o: getmem.c mem.h
	gcc -Wall -std=c11 -g -c getmem.c

freemem.o: freemem.c mem.h
	gcc -Wall -std=c11 -g -c freemem.c

get_mem_stats.o: get_mem_stats.c mem.h
	gcc -Wall -std=c11 -g -c get_mem_stats.c

print_heap.o: print_heap.c mem.h
	gcc -Wall -std=c11 -g -c print_heap.c

makeNewNode.o: makeNewNode.c mem.h
	gcc -Wall -std=c11 -g -c makeNewNode.c

mem_utils.o: mem_utils.c mem_impl.h
	gcc -Wall -std=c11 -g -c mem_utils.c

bench.o: bench.c mem.h mem_impl.h
	gcc -Wall -std=c11 -g -c bench.c

# runs 'bench' with default parameters
test: bench
	./bench

# cleans up .o files, temp files, and bench executable
clean:
	rm -f *.o bench *~
