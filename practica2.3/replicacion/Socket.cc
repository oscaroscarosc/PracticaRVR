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
    
    return(a->sin_family == b->sin_family && a->sin_addr.s_addr == b->sin_addr.s_addr && a->sin_port == b->sin_port);
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
	hints.ai_socktype = SOCK_STREAM;

	int rc = getaddrinfo(address, port, &hints, &res);
    if(rc != 0){
        std::cout << "Error: " << gai_strerror(rc) << "\n";
    }
    sd = socket(res->ai_family, res->ai_socktype, 0);

    listen(sd, 5);
}

// ----------------------------------------------------------------------------

int Socket::bind()
{
    return ::bind(sd, (const struct sockaddr *) &sa, sa_len);
}

// ----------------------------------------------------------------------------

int Socket::send(Serializable * obj, Socket * sock)
{
	return sendto(sd, obj->data(), obj->size(), 0, (struct sockaddr *) &sa, sa_len);
}

// ----------------------------------------------------------------------------

int Socket::recv(char * buffer, Socket ** sock)
{
	char host[NI_MAXHOST];
	char serv [NI_MAXSERV];
    
    int s = recvfrom(sd, buffer, 256, 0, (struct sockaddr *) &sa, &sa_len);
    buffer[s]='\0';

    getnameinfo((struct sockaddr *) &sa, sa_len, host, NI_MAXHOST,serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV);
    
    *sock = new Socket(sa, serv);

    return 1;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

