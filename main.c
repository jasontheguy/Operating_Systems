#include "mem.h"
#include <stdio.h>


int main(int argc, char** argv) {
  
  int r, t, size, duration, units, runs, seed, times;// input variables
  // First fit variables
  int first_probes=0, first_frags=0, first_average_probes=0, first_average_failures=0, first_average_frags=0, first_total_probes=0, first_total_failures=0, first_total_frags=0;
  //Next fit variable
  int next_probes=0, next_frags=0, next_average_probes=0, next_average_failures=0, next_average_frags=0, next_total_probes=0, next_total_failures=0, next_total_frags=0;
  //Best fit variable
  int best_probes=0, best_frags=0, best_average_probes=0, best_average_failures=0, best_average_frags=0, best_total_probes=0, best_total_failures=0, best_total_frags=0;

  units = atoi(argv[1]);
  seed = atoi(argv[4]);
  mem_init(units);
  srand(seed);
  runs = atoi(argv[2]);
  times = atoi(argv[3]);
  
  for(r=0;r<runs;r++) {
    // FIRSTFIT
    for(t=0;t<times;t++) {
      size = rand() % MAX_REQUEST_SIZE + MIN_REQUEST_SIZE;
      duration = rand() % MAX_DURATION + MIN_DURATION;
      first_probes = mem_allocate(FIRSTFIT,size,duration);
      if(first_probes != -1) {
        first_total_probes+=first_probes;
      }
      else if(first_probes == -1) {
        first_total_failures++;
      }
      first_frags = mem_fragment_count(MIN_REQUEST_SIZE-1);
      first_total_frags+=first_frags;

      mem_single_time_unit_transpired();
    }
    mem_clear();

    // NEXTFIT
    for(t=0;t<times;t++) {
      size = rand() % MAX_REQUEST_SIZE + MIN_REQUEST_SIZE;
      duration = rand() % MAX_DURATION + MIN_DURATION;
      next_probes = mem_allocate(NEXTFIT,size,duration);

      if(next_probes != -1) {
        //get average
        next_total_probes+=next_probes;
      }
      else if(next_probes == -1) {
        next_total_failures++;
      }
      next_frags = mem_fragment_count(MIN_REQUEST_SIZE-1);
      next_total_frags+=next_frags;

      mem_single_time_unit_transpired();
    }
    mem_clear();

    // BESTFIT
    for(t=0;t<times;t++) {
      size = rand() % MAX_REQUEST_SIZE + MIN_REQUEST_SIZE;
      duration = rand() % MAX_DURATION + MIN_DURATION;
      best_probes = mem_allocate(BESTFIT,size,duration);
      
      if(best_probes != -1) {
        //average the probes
        best_total_probes+=best_probes;
      }
      else if(best_probes == -1) {
        best_total_failures++;
      }
      best_frags = mem_fragment_count(MIN_REQUEST_SIZE-1);
      best_total_frags+=best_frags;

      mem_single_time_unit_transpired();
    }
    mem_clear();
  }

  first_average_frags = first_total_frags/runs;
  first_average_probes = first_total_probes/runs;
  first_average_failures = first_total_failures/runs;
  
  printf("\nFirst Fit");
  printf("\nAverage Probes: %i",first_average_probes);
  printf("\nAverage Failures %i",first_average_failures);
  printf("\nAverage Frags: %i",first_average_frags);

  next_average_frags = next_total_frags/runs;
  next_average_probes = next_total_probes/runs;
  next_average_failures = next_total_failures/runs;
  
  printf("\nAverage Fit");
  printf("\nAverage Probes: %i",next_average_probes);
  printf("\nAverage Failures %i",next_average_failures);
  printf("\nAverage Frags: %i",next_average_frags);

  best_average_frags = best_total_frags/runs;
  best_average_probes = best_total_probes/runs;
  best_average_failures = best_total_failures/runs;
  
  printf("\nBest Fit");
  printf("\nAverage Probes: %i",best_average_probes);
  printf("\nAverage Failures %i",best_average_failures);
  printf("\nAverage Frags: %i",best_average_frags);

  mem_free();
  return 0;
}
