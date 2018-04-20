#include <stdio.h>
#include <time.h>
#include <iostream>

int main () {
   time_t rawtime;
   struct tm *info;
   char buffer[80];

   time( &rawtime );

   info = localtime( &rawtime );

   strftime(buffer,80,"%Y", info);
   std::cout << "Estamos en el año : " << buffer;
   /*strftime(buffer,80,"%H", info);
   std::cout << "Estamos en el año : " << buffer;*/
  
   return(0);
}
