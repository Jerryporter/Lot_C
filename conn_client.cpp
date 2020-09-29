#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
	SOCKET s;
	sockaddr_in local, server;
	WSAData wsa;
	const char *buf = "I am a client";
	char recvbuf[128];

	WSAStartup(0x101, &wsa);

	s = socket(AF_INET, SOCK_STREAM, 0);
	local.sin_family = AF_INET;
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_port = htons(0x3412);

	bind(s, (sockaddr *)&local, sizeof(local));

	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
	server.sin_port = htons(0x1234);

	connect(s, (sockaddr *)&server, sizeof(server));

	send(s, buf, strlen(buf), 0);
	recv(s, recvbuf, sizeof(recvbuf), 0);
	cout << recvbuf << endl;
	closesocket(s);
	WSACleanup();

	system("pause");
	return 0;
}
