#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>

char * SVR_ADDR = "127.0.0.1";
int SVR_PORT = 1234;

using namespace std;

int main(int argc, char* args[])
{
	int sock;
	int ret;
	char buf[100];
	sockaddr_in serverAddr;

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

	read(sock, buf, 100);

	cout<<buf<<endl;

	close(sock);

	return 0;
}