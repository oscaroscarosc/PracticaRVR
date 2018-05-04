#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string.h>

int main (int argc, char** argv){
	struct addrinfo hints;
	struct addrinfo *res;

	memset((void*)&hints, '\0', sizeof(struct addrinfo));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	
	int rc = getaddrinfo(argv[1],argv[2],&hints,&res);
	if(rc != 0){
		std::cout << gai_strerror(rc) << std::endl;
		return -1;
	}

	int sd = socket(res->ai_family,res->ai_socktype,0);

	char buff[256];
	ssize_t size = sendto(sd,argv[3],2,0,(struct sockaddr*)res->ai_addr,res->ai_addrlen);
	ssize_t s = recvfrom(sd, buff, 255, 0,(struct sockaddr*)res->ai_addr,&res->ai_addrlen);
	buff[s] = '\0';

	std::cout << buff << "\n";
	freeaddrinfo(res);
	
	return 0;

	
}
