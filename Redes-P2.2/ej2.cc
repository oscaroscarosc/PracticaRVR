#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string.h>

int main (int argc, char** argv){
	struct addrinfo hints;
	struct addrinfo *res;

	memset((void*)&hints, '\0', sizeof(struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	
	int rc = getaddrinfo(argv[1],argv[2],&hints,&res);
	if(rc != 0){
		std::cout << gai_strerror(rc) << std::endl;
		return -1;
	}

	int sd = socket(res->ai_family,res->ai_socktype,0);
	bind(sd,res->ai_addr,res->ai_addrlen);

	freeaddrinfo(res);

	while(true){
		char buff[256];
		struct sockaddr src_addr;
		socklen_t addrlen = sizeof(src_addr);

		ssize_t s = recvfrom(sd,buff,255,0,&src_addr,&addrlen);
		char host[NI_MAXHOST];
		char serv[NI_MAXSERV];

		getnameinfo(&src_addr,addrlen,host,NI_MAXHOST,serv,NI_MAXSERV,NI_NUMERICHOST | NI_NUMERICSERV);

		std::cout << "Conexion: " << host << ":" << serv << "\n";
		std::cout << "Mensaje: " << buff << std::endl;

		sendto(sd,buff,s,0,&src_addr,addrlen);
	}
	return 0;

	
}
