#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS  5
int fecha (char c, char *buffer){
	time_t t;
   	struct tm *info;
   	time(&t);

   	info = localtime( &t );
	
	if (c == 't'){
		return strftime(buffer,80,"%r", info);
	}
	else if (c == 'd'){
		return strftime(buffer,80,"%D", info);
	}
	else if (c == 'q'){
		return 0;
	}
	else{
		std::cout << "Comando no soportado " << comando <<"\n";
		return -1;
	} 

}
class ServerThread{
public:
	ServerThread(int s): sd(s){}
	virtual ~ServerThread(){close(sd);}

	void do_message(){

		char buff[256];
		struct sockaddr src_addr;
		socklen_t addrlen = sizeof(src_addr);

		ssize_t s = recvfrom(sd,buff,255,0,&src_addr,&addrlen);
		char host[NI_MAXHOST];
		char serv[NI_MAXSERV];

		getnameinfo(&src_addr,addrlen,host,NI_MAXHOST,serv,NI_MAXSERV,NI_NUMERICHOST | NI_NUMERICSERV);
		std::cout << "Thread "<< pthread_self() << "\n"; 
		std::cout << "Conexion: " << host << ":" << serv << "\n";
		//std::cout << "Mensaje: " << buff << std::endl;

		int num = (fecha(buff[0], buff));
		sleep(10);
		if(num != -1){
			sendto(sd,buff,num,0,&src_addr,addrlen);
		}
	}
private:
	int sd;
};

extern "C"
{
	void* start_routine(void* _st){
		ServerThread * st = static_cast<ServerThread*>(_st);
		st->do_message();
		delete st;
		return 0;
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

	pthread_t tid[NUM_THREADS];	
	for(int i = 0; i < NUM_THREADS; i++){
		pthread_attr_t attr;

		ServerThread* st = new ServerThread(sd);
		pthread_attr_init (&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

		pthread_create(&tid[i], &attr, start_routine, static_cast<void*>(st));
	}

	char c;
	do std::cin >> c;
	while (c != 'q');
	freeaddrinfo(res);


	return 0;

	
}
