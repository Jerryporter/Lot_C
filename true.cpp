#include <iostream>
#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#include <string>
#include"nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;
int main(int argc, char* argv[])
{
	const std::string content = "On behalf of the Students 'Union, I am writing to extend my warm welcome to all the international students that are coming to our university. I firmly believe that your entering our university will bring us new ideas from different cultures and promote communication.";

	std::getline(std::cin,content);
	json root;

	root["PDU"]["protocol"] = "value";
	root["PDU"]["date"] = content;

	const std::string json_str = root.dump();

	int length_str = json_str.length();
	cout << length_str << endl;


	SOCKET s;
	sockaddr_in local, server;
	WSAData wsa;
	const char* buf = json_str.c_str();

	WSAStartup(0x101, &wsa);

	s = socket(AF_INET, SOCK_STREAM, 0);
	local.sin_family = AF_INET;
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_port = htons(0x3412);

	bind(s, (sockaddr*)&local, sizeof(local));

	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = inet_addr("192.168.2.74");
	server.sin_port = htons(8899);

	connect(s, (sockaddr*)&server, sizeof(server));

	send(s, (const char *)&length_str, sizeof((const char*)&length_str), 0);

	send(s, buf, strlen(buf), 0);

	closesocket(s);
	WSACleanup();

	system("pause");
	return 0;
}
