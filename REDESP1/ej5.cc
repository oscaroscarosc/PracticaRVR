#include <stdio.h>
#include <time.h>
#include <iostream>
#include <math.h>

int main () {
    struct timespec start, stop;
    long tiempoBucle;

    if(clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
      perror( "clock gettime" );
      return -1;
    }

    int i = 0;
    while(i<10000){
	++i;
    }

    if(clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
      perror( "clock gettime" );
      return -1;
    }

    tiempoBucle = (stop.tv_nsec - start.tv_nsec);

    /*tiempoBucle = ( stop.tv_sec - start.tv_sec );
    pow(tiempoBucle, -9);*/

    std::cout << tiempoBucle << std::endl;
    return(0);
}
