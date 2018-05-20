#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <unistd.h>


int main(int argc, char** argv){

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

	int sd = socket(res->ai_family,res->ai_socktype,6);
	if(sd == -1){
		std::cout << gai_strerror(sd) << std::endl;
		return -1;
	}

	connect(sd, (struct sockaddr *) res->ai_addr, res->ai_addrlen);	
	while(true){
		char buff[80];
		std::cin >> buff;
		ssize_t size = send(sd, buff, 80, 0);
		int c = 0;
		int i = 0;
		if (buff[0] != 'Q' || buff[1] != '\0') {
			memset(buff,0, 80);

			c = 0; i = 0;
			do {
			   	c = recv(sd, &(buff[i]), 1, 0);
			   	i++;
			} while ( c >= 0 && i < 79);
			  		
			std::cout << buff << "\n";
			memset(buff,0, 80);
		}
		else {
			shutdown(sd, SHUT_RDWR);
			c = 0; i = 0;
			do {
			   	c = recv(sd, &(buff[i]), 1, 0);
			   	i++;
			} while ( c >= 0 && i < 79);
			break;

		}
	}
	freeaddrinfo(res);
	return 0;
}
