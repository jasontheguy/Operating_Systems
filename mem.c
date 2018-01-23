#include <stdlib.h>   // for malloc() and free()
#include "mem.h"

/*
  Physical memory array. 
  An element in the array with a value of zero represents a free unit of memory.
*/
static unsigned int* memory;

/*
 The size of the physical memory array. 
*/
static unsigned int mem_size;


/*
 The last_placement_position variable contains the end position of the last 
 allocated unit used by the next fit placement algorithm.  
*/
static unsigned int last_placement_position;

/*
  Using the memory placement algorithm, strategy, allocate size
  units of memory that will reside in memory for duration time units.

  If successful, this function returns the number of blocks
  (a block is a contiguous "chuck" of units) of free memory probed while 
  searching for a suitable block of memory according to the placement 
  strategy specified.  If unsuccessful, return -1.

  If a suitable contiguous block of memory is found, the first size
  units of this block must be set to the value, duration.
*/
int mem_allocate(mem_strats_t strategy,int size, dur_t duration) {
  int count,i,start_block,num_blocks, best, best_position;
  i=0;
  count=0;
  start_block=0;
  num_blocks=0;

  // FIRSTFIT
  if(strategy == FIRSTFIT) {
    while(i<mem_size) {
      if((i!=0 && memory[i]==0 && memory[i-1] != 0) || (i==0 && memory[i] ==0)){//Check to see if free block found
        start_block=i;
        num_blocks++;
        count=0;
      }
      if(memory[i]==0) {//cell is empty
        count++;
      }
      else if(memory[i] != 0) {
        count=0;
      }
      if(i==mem_size-1 && count <= size) {// failing condition check
        return -1;
      }
      if(size <= count) {//first free block found
        while(count !=0) {
          memory[start_block] = duration;
          start_block++;
          count--;
        }
        break;
      }
      i++;
    } 
  }

  // NEXTFIT
  else if(strategy == NEXTFIT) {
    // Begin scan from last_placement_position
    while(i<mem_size) {
      if((i!=0 && memory[i]==0 && memory[i-1] != 0) || (i==0 && memory[i] ==0)) {
        // start of block
        start_block=i;
        num_blocks++;
        count=0;
      }
      if(memory[i]==0) {
        // count free units
        count++;
      }
      else if(memory[i] != 0) {
        count=0;
      }
      if(i==mem_size-1 && count <= size) {
        // failure
        return -1;
      }
      if(size <= count) {
        // free block
        last_placement_position = start_block;
        i=last_placement_position;
        while(count !=0) {
          memory[start_block] = duration;
          start_block++;
          count--;
        }
        break;
      }
      if(last_placement_position == mem_size-1) {
        // reached end, reset
        last_placement_position=0;
      }
      i++;
    } 
  }

  // BEST
  else if(strategy == BESTFIT) {
    while(i<mem_size) {
      if((i!=0 && memory[i]==0 && memory[i-1] != 0) || (i==0 && memory[i] ==0)) {//free block found
        start_block=i;
        num_blocks++;
        count=0;
      }
      if(memory[i]==0) {// cell is empty
        count++;
      }
      else if(memory[i] != 0) {
        count=0;
      }
      if(i==mem_size-1 && count <= size) {// failing condition
        return -1;
      }
      if(size <= count) {// checks best position value and updates
        best = size-count;
        if(best >= 0) {
          best_position = start_block;
        } 

        if(count < best) {//new block found is better than old starting block
          best_position = start_block; 
        }
        else {
          while(count !=0) {
            memory[best_position] = duration;
            best_position++;
            count--;
          }
        }
        break;
      }
      i++;
    }
  }
  return num_blocks;
}

/*
  Go through all of memory and decrement all positive-valued entries.
  This simulates one unit of time having transpired.
*/
int mem_single_time_unit_transpired() {
  int i=0;
  while(i<mem_size) {
    if(memory[i]>0) {
      memory[i] = memory[i]-1;
    }
    i++;
  }
  return 0;
}

/*
  Return the number of fragments in memory.  A fragment is a
  contiguous free block of memory of size less than or equal to
  frag_size.
*/
int mem_fragment_count(int frag_size) { 
  int i = 0;
  int num_frags = 0;
  int num_free = 0;
  while(i<mem_size) {
    if(memory[i] == 0) { //Found a free spot
      num_free++;
    }
    if(i>0 && memory[i] != 0 && memory[i-1] == 0 ) {// end of block, compare size free and frag size vals
      if(num_free < frag_size ) {
        num_frags++;
      }
      num_free=0;
    }
    i++;
  }
  return num_frags;
}

/*
  Set the value of zero to all entries of memory.
*/
void mem_clear() {
  int i=0;
  while(i<mem_size) {
    memory[i]=0;
    i++;
  }
}

/*
 Allocate physical memory to size.
*/
void mem_init(int size)
{
	memory = malloc( sizeof(int)*size );
	mem_size = size;
}

/*
 Deallocate physical memory.
*/
void mem_free()
{
	free( memory );
}
