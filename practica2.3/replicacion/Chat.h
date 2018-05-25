#include <string>
#include <unistd.h>
#include <string.h>
#include "Serializable.h"
#include "Socket.h"
#include "UDPServer.h"

class ChatMessage: public Serializable
{
public:
    ChatMessage(){};

    ChatMessage(const char * n, const std::string m)
    {
	strncpy(nick, n, 8);
	m.copy(message,80);
    };

    void to_bin()
    {
	int32_t total = 80 * sizeof(char) + 2 * sizeof(int16_t);
    	alloc_data(total);

    	char * tmp = _data + sizeof(int32_t);
    	memcpy(tmp, nick, 8* sizeof(char));
    	tmp += 8 * sizeof(char);
    	memcpy(tmp, message, 80* sizeof(char));
    	tmp += 80 * sizeof(char);
    }

    virtual int from_bin(char * bobj)
    {
	char *tmp = bobj + sizeof(int32_t);
	memcpy(nick, tmp, 8* sizeof(char));
	tmp += 8 * sizeof(char);
	memcpy(message, tmp, 80* sizeof(char));
	tmp += 80 * sizeof(char);
	return 0;
    }

    char message[80];

    char nick[8];
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class ChatServer : public UDPServer
{
public:
    ChatServer(const char * s, const char * p): UDPServer(s, p){};

    void do_message(char * buffer)
    {
    	ChatMessage chtMsg;
    	chtMsg.from_bin(buffer);
    	for(int i = 0; i < connections.size(); i++){
    		socket.send(&chtMsg, connections[i]);
    	}
    }
};

class ChatClient
{
public:
    ChatClient(const char * s, const char * p, const char * n):socket(s, p){
	strncpy(nick, n, 8);
    };

    void input_thread()
    {
    	char aux[80];
    	while(true){
	    ChatMessage* chtMsg;
    	    std::cin >> aux;
    	    chtMsg = new ChatMessage(nick, aux);
    	    chtMsg->to_bin();
    	    socket.send(chtMsg, &socket);
    	}
    }

    void net_thread()
    {
    	char buff[80];
    	ChatMessage chtMsg;

    	while(true){
		socket.recv(buff);
		chtMsg.from_bin(buff);
		if(chtMsg.nick != nick){
			std::cout << chtMsg.nick <<  "    -   "<< chtMsg.message;
		}
	}
    }

private:
    char nick[8];

    Socket socket;
};

