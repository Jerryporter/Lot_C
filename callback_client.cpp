#include "frequentuse.h"

using namespace std;

void clientThread();

int main(int argc, char const *argv[])
{
    SOCKET s;
    sockaddr_in local, server;
    WSAData wsa;

    WSAStartup(0x101, &wsa);

    s = socket(AF_INET, SOCK_STREAM, 0);
    local.sin_family = AF_INET;
    local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    local.sin_port = htons(0x3412);

    bind(s, (sockaddr *)&local, sizeof(local));
    server.sin_family = AF_INET;
    server.sin_addr.S_un.S_addr = inet_addr("192.168.2.74");
    server.sin_port = htons(8899);

    connect(s, (sockaddr *)&server, sizeof(server));

    WINSOCK_API_LINKAGE int PASCAL (*sendData)(SOCKET, const char *, int, int);

    system("pause");
    return 0;
}

void clientThread(SOCKET &s)
{
    
}