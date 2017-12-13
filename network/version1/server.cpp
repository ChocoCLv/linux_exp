#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

int SVR_PORT = 1234;

int main(int argc, char * args[])
{
	int serverSock;
	int clientSock;
	int ret;
	char resp[100] = "chenlv 201722010533";
	sockaddr_in serverAddr;
	sockaddr_in clientAddr;
	socklen_t addrlen;

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
	serverAddr.sin_port = SVR_PORT;

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

	addrlen = sizeof(sockaddr_in);
	clientSock = accept(serverSock, (sockaddr*)&serverAddr, &addrlen);
	if(clientSock == -1)
	{
		cout<<"accept error"<<endl;
		return -1;
	}

	write(clientSock, resp, 100);

	close(clientSock);
	close(serverSock);
}