#include <stdio.h>
#include <time.h>
#include <iostream>

int main () {
   time_t seconds;

   seconds = time(NULL);
   std::cout << ctime(&seconds) << std::endl;
  
   return(0);
}
