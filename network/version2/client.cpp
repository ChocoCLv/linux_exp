#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>

using namespace std;

class Client
{
private:
	int sock;
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
		inet_pton(AF_INET, addr, &serverAddr.sin_addr);
		serverAddr.sin_port = port;

		ret = connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
		if(ret == -1)
		{
			cout<<"connect error:"<<strerror(errno)<<endl;
			return -1;
		}
	}
	
	int readData(char *buf, int bufSize)
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
};

int main(int argc, char* args[])
{
	Client client;
	char buf[100];
	client.connectToServer("127.0.0.1", 1234);
	client.readData(buf, 100);
	cout<<buf<<endl;
	client.disconnectFromServer();
	return 0;
}