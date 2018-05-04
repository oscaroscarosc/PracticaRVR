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
	hints.ai_socktype = SOCK_STREAM;
	
	int rc = getaddrinfo(argv[1],argv[2],&hints,&res);
	if(rc != 0){
		std::cout << gai_strerror(rc) << std::endl;
		return -1;
	}

	int sd = socket(res->ai_family,res->ai_socktype,0);
	bind(sd,res->ai_addr,res->ai_addrlen);

	listen(sd,7);

	char buff[256];
	struct sockaddr src_addr;
	socklen_t addrlen = sizeof(src_addr);
	char host[NI_MAXHOST];
	char serv[NI_MAXSERV];
	int socket_descriptor;

	//freeaddrinfo(res);

	while(true){
		
		socket_descriptor = accept(sd,(struct sockaddr*)&src_addr,&addrlen);
		getnameinfo(&src_addr,addrlen,host,NI_MAXHOST,serv,NI_MAXSERV,NI_NUMERICHOST | NI_NUMERICSERV);

		std::cout << "Conexion: " << host << ":" << serv << "\n";
		//std::cout << "Mensaje: " << buff << std::endl;

		int c = 0;
		do {
        		c = recv(cliente_sd, &(buffer[i]), 1, 0);
  		} while ( c >= 0 && i < 79 && buffer[i++] != '\n');

	
		//sendto(sd,buff,num,0,&src_addr,addrlen);
	
	}
	return 0;

	
}
