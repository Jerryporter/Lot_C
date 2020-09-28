// ����������.cpp : �������ӣ��յ��ͻ������������ַ����󣬻���һ��ACK�ַ������ͻ�����
//
#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#include<iostream>
using namespace std;
int main(int argc, char *argv[])
{
	SOCKET s, newsock;
	sockaddr_in ser_addr;
	sockaddr_in remote_addr;
	int len;
	char buf[128];
	WSAData wsa;

	WSAStartup(0x101, &wsa);
	s = socket(AF_INET, SOCK_STREAM, 0);
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	ser_addr.sin_port = htons(0x1234);

	bind(s, (sockaddr *)&ser_addr, sizeof(ser_addr));

	listen(s, 0);
	while (1)
	{
		len = sizeof(remote_addr);
		newsock = accept(s, (sockaddr *)&remote_addr, &len);
		recv(newsock, buf, sizeof(buf), 0);
		cout << buf << endl;
		send(newsock, "ACK", 3, 0);
		closesocket(newsock);
	}
	closesocket(s);
	WSACleanup();
	return 0;
}
