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
   std::cout << "Estamos en el año: " << buffer;

   setlocale(LC_ALL, "es_ES.UTF-8");
   strftime(buffer,80,"%A", info);
   std::cout << "\nHoy es " << buffer;

   strftime(buffer,80,"%H:%M", info);
   std::cout << ", " << buffer << "\n";
  
   return(0);
}
