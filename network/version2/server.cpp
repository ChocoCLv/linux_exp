#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

class Server
{
private:
	int serverSock;
	int clientSock;
	int port;
	sockaddr_in serverAddr;
	sockaddr_in clientAddr;

public:
    Server(int _port)
	{
		port = _port;
	}

	int startlisten()
	{
		int ret;
		//create server listen socket
		serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if( serverSock == -1 )
		{
			cout<<"create socket error:"<<strerror(errno)<<endl;
			return -1;
		}

		//set server address
		memset(&serverAddr, 0 ,sizeof(sockaddr_in));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		serverAddr.sin_port = port;

		//bind socket with addr
		ret = bind(serverSock, (sockaddr*)&serverAddr, sizeof(sockaddr_in));
		if(ret == -1)
		{
			cout<<"bind socket error"<<endl;
			return -1;
		}

		ret = listen(serverSock, 5);
		if(ret == -1)
		{
			cout<<"listen error"<<endl;
			return -1;
		}
	}
	
	int acceptClient()
	{
		socklen_t addrlen;
		addrlen = sizeof(sockaddr_in);
		clientSock = accept(serverSock, (sockaddr*)&serverAddr, &addrlen);
		if(clientSock == -1)
		{
			cout<<"accept error"<<endl;
			return -1;
		}

		return clientSock;
		
	}

	int send(int sockId, char *data, int len)
	{
		write(sockId, data, len);
	}

	~Server()
	{
		cout<<"close sock"<<endl;
		close(clientSock);
		close(serverSock);
	}
};

int main(int argc, char * args[])
{
	int clientSock;
	char resp[100] = "chenlv 201722010533";
	Server server(1234);
	server.startlisten();
	clientSock = server.acceptClient();
	server.send(clientSock, resp, 100);
	return 0;
}