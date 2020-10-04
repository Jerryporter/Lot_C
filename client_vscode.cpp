// coclient.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <winsock.h>
#include "mingw-threads/mingw.thread.h"
#include <chrono>
#pragma comment(lib, "wsock32.lib")
#include "nlohmann/json.hpp"
#include "frequentuse.h"
#include "timer.h"

using json = nlohmann::json;
using namespace std::chrono;
#define SERVER_PORT htons(8899)
#define SERVER_IP inet_addr("192.168.2.74")
static int stateWrite = 0;
static int stateKeepAlive = 0;

string genKeepAliveMess();
// DWORD WINAPI SendKeepAliveMess();
// DWORD WINAPI getKeybroadInput();
void waitTimeToSendKeepAliveMess();
void getKeybroadInput();
int main(int argc, char *argv[])
{
    SOCKET sock;
    struct sockaddr_in server;
    WSAData wsa;
    int retval;

    WSAStartup(0x101, &wsa);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        printf("Creating socket error\n");
        return 0;
    }

    server.sin_family = AF_INET;
    server.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK); //inet_addr("202.115.12.38");
    server.sin_port = htons(0x1234);

    // if (connect(sock, (sockaddr *)&server, sizeof(server)) < 0)
    // {
    //     retval = WSAGetLastError();
    //     printf("Conneting to server error\n");
    //     return 0;
    // }

    retval = 1;
    printf("connect to server\n");

    std::thread clockThread(waitTimeToSendKeepAliveMess); // pass by value
    clockThread.join();
    while (1)
    {
        if (stateKeepAlive)
        {
            string mess = genKeepAliveMess();
            int messLength = mess.length();
            //send(sock,(const char *)&messLength,messLength,0);
            send(sock, mess.c_str(), mess.length(), 0);
            stateKeepAlive=0;
        }
        if (stateWrite)
        {
            string mess = "On behalf of the Students 'Union, I am writing to extend my warm welcome to all the international students that are coming to our university. I firmly believe that your entering our university will bring us new ideas from different cultures and promote communication.";
            send(sock, mess.c_str(), mess.length(), 0);
            stateWrite = 0;
        }

        // if (stateSocket = 1)
        // {
        //     string mess = genKeepAliveMess().dump();

        //     send(sock, mess.c_str(), mess.length(), 0);
        //     stateKeepAlive = 0;
        // }
    }

    //close the socket

    closesocket(sock);
    WSACleanup();

    system("pause");

    return 0;
}

string genKeepAliveMess()
{
    json mess;
    mess = R"({"protocol " : " keepalive "})"_json;
    return mess.dump();
}
void waitTimeToSendKeepAliveMess()
{
    int stateKeepAlive = 0;
    int i = 0;
    time_t tt;
    cout << "clock set up!" << endl;
    while (1)
    {
        chrono::steady_clock::time_point start = chrono::steady_clock::now();
        while (!stateKeepAlive)
        {
            chrono::steady_clock::time_point end = chrono::steady_clock::now();
            auto time_span = chrono::duration_cast<chrono::seconds>(end - start).count();
            if (time_span >= 2)
            {
                stateKeepAlive = 1;
                chrono::system_clock::time_point today = chrono::system_clock::now();
                tt = chrono::system_clock::to_time_t(today);
                cout << ctime(&tt);
                i++;
            }
        }
        //stateKeepAlive = 0;
        if (i == 5)
        {
            return;
        }
    }
}
void getKeyboardInput()
{
}

// DWORD WINAPI waitTimeToSendKeepAliveMess()
// {
//     cout<<"clock set up!"<<endl;
//     while (1)
//     {
//         steady_clock::time_point start = steady_clock::now();
//         while (!stateKeepAlive)
//         {
//             steady_clock::time_point end = steady_clock::now();
//             auto time_span = duration_cast<seconds>(end - start).count();
//             if (time_span>60)
//             {
//                 stateKeepAlive=1;
//             }
//         }
//     }
//     return 0;
// }
// DWORD WINAPI getKeybroadInput()
// {
//     SOCKET sock;
//     int i;
//     char buf[80];
//     gets(buf);
//     sock = *(SOCKET *)lpParam;
//     printf("thread get socket = %d  \n", sock);
//     while (1)
//     {
//         for (i = 0; i < 2000000; i++)
//             ; /* you can test how the system schedule between threads*/
//     }
//     printf("close a socket\n");
//     closesocket(sock);
//     return 0;
// }
