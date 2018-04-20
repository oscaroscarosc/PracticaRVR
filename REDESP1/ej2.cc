#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>

int main() {
   /* Comprobar la ocurrencia de error y notificarlo con la llamada perror(3) */      
    int rc = setuid(2);
    if(rc != 0){
	perror("Error: ");
	std::cout << "El error es: " << strerror(errno) << std::endl;
	return -1;
    }
    return 1;
}
