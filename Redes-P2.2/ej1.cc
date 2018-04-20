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
	
	int rc = getaddrinfo(argv[1],0,&hints,&res);
	if(rc != 0){
		std::cout << gai_strerror(rc) << std::endl;
		return -1;
	}

	for(struct addrinfo* tmp = res; tmp != 0; tmp = tmp->ai_next){
		char host[NI_MAXHOST];
		char serv[NI_MAXSERV];

		getnameinfo(tmp->ai_addr,tmp->ai_addrlen,host,NI_MAXHOST,serv,NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
		std::cout << "Host: " << host << "   " << tmp->ai_family << "    " << tmp->ai_socktype << "\n";
	}
	freeaddrinfo(res);
	return 0;

	
}
