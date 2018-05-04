#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string.h>

int fecha (char comando, char *buffer){
	time_t rawtime;
   	struct tm *info;
   	//char buffer[80];

   	time( &rawtime );

   	info = localtime( &rawtime );

   	
	if (comando == 't'){
		return strftime(buffer,80,"%r", info);
	}
	else if (comando == 'd'){
		return strftime(buffer,80,"%D", info);
	}
	else if (comando == 'q'){
		return 0;
	}
	else{
		std::cout << "Comando no soportado " << comando <<"\n";
		return -1;
	} 

}
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
		//std::cout << "Mensaje: " << buff << std::endl;

		int num = (fecha(buff[0], buff));
		if (num == 0){
			std::cout << "Saliendo...";
			freeaddrinfo(res);
			return 0;
		}
	
		sendto(sd,buff,num,0,&src_addr,addrlen);
	
	}
	return 0;

	
}
