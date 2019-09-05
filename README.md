## CSE 374 - HW6
#### by Tyler Ho and Alex Htut

Our repository (**cse374-19sp-students-htutk-tmhh**) is located [here](https://gitlab.cs.washington.edu/cse734-19sp-students/cse374-19sp-students-htutk-tmhh).

### Work Division
Tyler:
  - _100%_ of **freemem.c**
  - _90%_ of **bench.c**
  - _100%_ of **mem_utils.c**

Alex:
  - _100%_ of **getmem.c**
  - _10%_ of **bench.c**: adding xFE and removing them to/from the pointers used by getmem/freemem.
  - _100%_ of **print_heap.c**
  - _100%_ of **get_mem_stats.c**
 
The functions to the header files were added by both of us as we went along.

### FreeList Implementation
The **freeList** is implemented as a _linked_ _list_ as suggested. Each node has three fields: size, address, and next node.
There are some guaranteed properties of each node data due to the ways getmem() and freemem() are implemented:
1. The size will _never be less than 16_ since _getmem_ makes sure that the blocks are 16-byte aligned. 
This also means that everytime the client requests some amount less than 16, they will receive at least 16 bytes.
2. The hex address will also end with _0_ since they are 16-byte aligned.
No two nodes will have adjacent addresses as assertions in check_cheap() ensure it.

### Features and/or Improvements
Getmem():
- When the freeList does not have any more valid memory block, the default size, getmem() requests is _4K (4096)_.
If the client requests something larger than that, getmem() will exactly get that size and return it directly to the client.
Note: every malloc requests extra 16-byte to store the size information (which requires 8 bytes); 8 more bytes is requested for alignment.

- Getmem() has a simple algorithm. Everytime getmem() is called, it checks the freeList (in the ascending order of address) to see if there is any block that can satisfy the client's request.
If such block is found, getmem() will _free_ that block from the freeList. If the block is much larger (32-byte or more) than the request, getmem() will split and update the block's info for later use. If not, it will return the whole block.
If there is no valid block in the freeList, getmem() will get an appropriate size (`getSize = size < 4K ? 4K : size`) from the underlying system; try to split if possible and return the address to the client.

Freemem():
- As required by the specs, freemem() will attempt to combine the adjacent blocks within the freeList if possible.
It will first add the _freed_ node to an appropriate spot (maintaining the ascending order of address).
Then, it goes through the updated freeList and tries to combine the two adjacent nodes together. Note that this can be done in **n runtime** since the address order is maintained.
However, there may be such a case where the recently freed block was taken from the "_middle_ of a block" (which will only happen with right order of getmem() and freemem()). So, freemem() tries to combine those again with another function call.
Therefore, the total runtime to combine adjacent nodes becomes **2n runtime**.

Extra Credit: not attempted.

### Demo Analysis 
Runtime:
- The default _ntrials=10000_ finishes at 0.0625s. Note that this experiment is including print_heap(), which writes to an output text file. Without print_heap(), the program finishes instantaneously.
Without print_heap(), _ntrials=1M_ terminates successfully under 2s!
Note: if you set pctlarge to a large percentage, the runtime will increase as it needs to malloc() more time. Previous experiments are done with default pctlarge=10

Efficiency:
- After the output.txt (txt file produced from print_heap()) is analyzed a few times from different experiments, our heap seems to be left with small(er) blocks which are unlikely to used by the client again (unless the client requests particularly small blocks).
- This is not too efficient since every time getmem() or freemem() is called, the iteration will have to go through those 'useless' blocks (which are just there 'doing nothing').
One way to solve this 'issue' is to set the threshold (32-byte in our program) to a higher number. That way, the 'whole' block is returned and no smaller remainder is left behind.
Another way (as mentioned in the extra credit section) is to 'remember' the node that was splitted/freed from the freeList and continue the search afterward when getmem() is called. This way, randomness in freeing will become less of a factor.
Also, if randomness is reduced/removed, there is a better way to handle: getmem() and freemem() in a ordered fashion, and there will be little or no small blocks.

### Resources
- General resources such as [stackoverflow](https://stackoverflow.com), [geeksforgeeks](https://www.geeksforgeeks.org/), [tutorialpoints](https://www.tutorialspoint.com/).
- CSE374's [HW6](https://courses.cs.washington.edu/courses/cse374/19sp/homework/hw6.html) description on general basic idea.

