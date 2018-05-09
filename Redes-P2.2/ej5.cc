#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

int main (int argc, char** argv){
	struct addrinfo hints;
	struct addrinfo *res;

	memset((void*)&hints, '\0', sizeof(struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	
	int rc = getaddrinfo(argv[1],argv[2],&hints,&res);
	if(rc != 0){
		std::cout << gai_strerror(rc) << std::endl;
		return -1;
	}

	int sd = socket(res->ai_family,res->ai_socktype,0);

	if(sd == -1){
		std::cout << "Error: " <<  gai_strerror(sd) << std::endl;
		return -1;
	}

	int error = connect(sd, (struct sockaddr *) res->ai_addr, res->ai_addrlen);
	if(error != 0){
		std::cout << "Error: " << gai_strerror(error) << std::endl;
		return -1;
	}

	
	while(true){
		char buff[256];
		int i = 0;
		int c = 0;
		std::cin >> buff;

		ssize_t size = send(sd, buff, 255, 0);

		if (buff[0] != 'q' && buff[1] != '\0') {
			memset(buff,0, 255);

			c = 0; i = 0;
			do {
			   	c = recv(sd, &(buff[i]), 1, 0);
			   	i++;
			} while ( c >= 0 && i < 255);
			  		
			std::cout << buff << "\n";
			memset(buff,0, 255);
		}
		else {
			shutdown(sd, SHUT_RDWR);
			c = 0; i = 0;
			do {
			   	c = recv(sd, &(buff[i]), 1, 0);
			   	i++;
			} while ( c >= 0 && i < 255);
			break;
		}
	}
	freeaddrinfo(res);
	return 0;
}
