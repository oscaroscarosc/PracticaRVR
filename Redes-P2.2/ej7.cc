#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

#define NUM_THREADS  5

class ServerThread{
public:
	ServerThread(int s): sd(s){}
	virtual ~ServerThread(){close(sd);}

	void do_message(){

		char buffer[256];

		while (true){
	  		int c = 0; int i = 0;
	  		do {
	        	c = recv(sd, &(buffer[i]), 1, 0);
	  		} while ( c >= 0 && i < 255 && buffer[i++] != '\n');
	  		
	  		if(c == 0)
	  		{
	  			close(sd);
	  			std:: cout << "Conexión terminada\n";
	  			pthread_cancel(pthread_self());
	  			break;
	  		} 
	  		send(sd, buffer, i, 0);
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


int main(int argc, char** argv){

	struct addrinfo hints;
	struct addrinfo* res;
	memset((void*) &hints,'\0', sizeof(struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);
	if(rc != 0){
		std::cout << "Error: " << gai_strerror(rc) << std::endl;
		return -1;
	}
	int sd = socket(res->ai_family, res->ai_socktype, 0);
	bind(sd,res->ai_addr, res->ai_addrlen);

	listen(sd, 7);
	
	struct sockaddr src_addr;
	socklen_t addrlen = sizeof(struct sockaddr);
	char host[NI_MAXHOST];
	char serv [NI_MAXSERV];
	int socket_descriptor;
	
	freeaddrinfo(res);

	while(true){

		
  		socket_descriptor = accept(sd, (struct sockaddr *) &src_addr, &addrlen);

  		getnameinfo((struct sockaddr *) &src_addr, addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, 
  			NI_NUMERICHOST|NI_NUMERICSERV);
  
		//std::cout << "Thread "<< pthread_self() << "\n";
		std::cout << "Conexion: " << host << ":" << serv << "\n";

  		pthread_t tid;
  		pthread_attr_t attr;

		ServerThread* st = new ServerThread(socket_descriptor);
		pthread_attr_init (&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

		pthread_create(&tid, &attr, start_routine, static_cast<void*>(st));

  		}

  		return 0;
	}
