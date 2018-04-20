#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main() {
   /* Comprobar la ocurrencia de error y notificarlo con la llamada perror(3) */      
    int rc = setuid(2);
    if(rc != 0){
	perror("Error: ");
	return -1;
    }
    return 1;
}

