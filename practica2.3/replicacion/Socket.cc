#include "Socket.h"
#include "Serializable.h"

#include <string.h>

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Implementación Socket
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

bool operator== (const Socket &s1, const Socket &s2)
{
    struct sockaddr_in * aux1, * aux2;
    aux1 = (struct sockaddr_in *)&s1.sa;
    aux2 = (struct sockaddr_in *)&s2.sa;

    if (aux1->sin_family == aux2->sin_family && aux1->sin_addr.s_addr == aux2->sin_addr.s_addr && aux1->sin_port == aux2->sin_port){
    	return true;
    }
    else return false;
}

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;
}

// ----------------------------------------------------------------------------

Socket::Socket(const char * address, const char * port):sd(-1)
{
	struct addrinfo hints;
	struct addrinfo* res;
	memset((void*) &hints,'\0', sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	int rc = getaddrinfo(address, port, &hints, &res);
    	if(rc != 0){
        	std::cout << "Error: " << gai_strerror(rc) << "\n";
    	}
    	sd = socket(res->ai_family, res->ai_socktype, 0);
}

// ----------------------------------------------------------------------------

int Socket::bind()
{
    return ::bind(sd, (const struct sockaddr *) &sa, sa_len);
}

// ----------------------------------------------------------------------------

int Socket::send(Serializable * obj, Socket * sock)
{
	obj->to_bin();
	return sendto(sd, obj->data(), obj->size(), 0, (struct sockaddr *) &sa, sa_len);
}

// ----------------------------------------------------------------------------

int Socket::recv(char * buffer, Socket ** sock)
{

	struct sockaddr src;
	socklen_t src_len = sizeof(struct sockaddr);

	if(sock != 0){
		int size =  recvfrom(sd, &buffer, MAX_MESSAGE_SIZE, 0, &src, &src_len);
		*sock = new Socket(&src,src_len);
	}

    return 0;

}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

