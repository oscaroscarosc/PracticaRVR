#include "UDPServer.h"
#include "Serializable.h"

UDPServer::UDPServer(const char * serv, const char * port)
    :socket(serv, port)
{
    pthread_mutex_init(&mutex, 0);
};

UDPServer::~UDPServer()
{
    pthread_mutex_destroy(&mutex);
};

// ----------------------------------------------------------------------------

extern "C" void * _server_thread(void *arg)
{
    UDPServer * server = static_cast<UDPServer *>(arg);

    server->server_thread();

    return 0;
}

// ----------------------------------------------------------------------------

int UDPServer::start()
{
	
	pthread_t tid[THREAD_POOL_SIZE];	
	for(int i = 0; i < THREAD_POOL_SIZE; i++){
		pthread_attr_t attr;

		pthread_attr_init (&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

		pthread_create(&tid[i], &attr, _server_thread, static_cast<void*>(this));
	}
	return 0;
}

// ----------------------------------------------------------------------------

void UDPServer::server_thread()
{
	char buff [256];
	while(true){
		Socket * sClient;
		socket.recv(buff, &sClient); 
		connections.push_back(sClient);
		do_message(buff);
	  	/*std::cout << "Thread: " << pthread_self() << "\n";
	  	std::cout << "Conexión : "<< host << " Puerto: "<< serv<<"\n";*/
	}
}

// ----------------------------------------------------------------------------

void UDPServer::add_connection (Socket * s)
{
	pthread_mutex_lock(&mutex);
	int aux = 0;
	bool found = false;
	while(aux < connections.size() && !found){
		if (connections[aux] != s){
			aux++;
		}
		else found = true;
	}

	if (aux >= connections.size()){
		if(connections.size() <= THREAD_POOL_SIZE){
			connections.push_back(s);
		}
	}

	else delete s;

	pthread_mutex_unlock(&mutex);

}

// ----------------------------------------------------------------------------

void UDPServer::del_connection (Socket * s)
{
	pthread_mutex_lock(&mutex);
	int aux = 0;
	bool found = false;
	while(aux < connections.size() && !found){
		if (connections[aux] != s){
			aux++;
		}
		else found = true;
	}
	if(aux < connections.size())
		connections.erase(connections.begin()+aux);

	pthread_mutex_unlock(&mutex);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

