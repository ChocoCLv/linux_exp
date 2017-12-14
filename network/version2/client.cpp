#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>

class Client
{
private:
	int sock = 0;
	char buf[100];
	sockaddr_in serverAddr;
public:
	int connectToServer(char * addr, int port)
	{
		int ret;
		//create socket
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if( sock == -1 )
		{
			cout<<"create socket error:"<<strerror(errno)<<endl;
			return -1;
		}

		//initial server address
		memset(&serverAddr, 0, sizeof(sockaddr_in));

		serverAddr.sin_family = AF_INET;
		inet_pton(AF_INET, SVR_ADDR, &serverAddr.sin_addr);
		serverAddr.sin_port = SVR_PORT;

		ret = connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
		if(ret == -1)
		{
			cout<<"connect error:"<<strerror(errno)<<endl;
			return -1;
		}
	}
	
	int read(char *buf, int bufSize)
	{
		return read(sock, buf, 100);
	}

	int disconnectFromServer()
	{
		if(sock == -1|| sock == 0)
		{
			return -1;
		}
		close(sock);
	}

	
}