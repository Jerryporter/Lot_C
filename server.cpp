#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>

int main()

{

    //第一步：加载socket库函数

    //**********************************************************

    WORD wVersionRequested;

    WSADATA wsaData;

    int err;

    wVersionRequested = MAKEWORD(1, 1);

    err = WSAStartup(wVersionRequested, &wsaData);

    if (err != 0)
    {

        return 0;
    }

    if (LOBYTE(wsaData.wVersion) != 1 ||

        HIBYTE(wsaData.wVersion) != 1)
    {

        WSACleanup();

        return 0;
    }

    //**********************************************************

    //第二步创建套接字

    SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

    //第三步：绑定套接字

    //获取地址结构

    SOCKADDR_IN addrSrv;

    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    //将IP地址指定为INADDR_ANY，允许套接字向任何分配给本地机器的IP地址发送或接收数据

    //htonl()将主机的无符号长整形数转换成网络字节顺序。

    addrSrv.sin_family = AF_INET;

    //sin_family 表示地址族，对于IP地址，sin_family成员将一直是AF_INET

    addrSrv.sin_port = htons(6000);

    //htons()将主机的无符号短整形数转换成网络字节顺序

    bind(sockSrv, (SOCKADDR *)&addrSrv, sizeof(SOCKADDR));

    //监听客户端

    listen(sockSrv, 5);

    //定义从客户端接受的地址信息

    SOCKADDR_IN addrClient;

    int len = sizeof(SOCKADDR);

    while (1)

    {

        //不断等待客户端的请求的到来，并接受客户端的连接请求

        printf("等待客户连接n");

        SOCKET sockConn = accept(sockSrv, (SOCKADDR *)&addrClient, &len);

        char sendBuf[100];

        sprintf(sendBuf, "welcome %s to wuhan", inet_ntoa(addrClient.sin_addr));

        printf("发送数据n");

        send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);

        char recvBuf[100];

        printf("等待接受数据n");

        recv(sockConn, recvBuf, 100, 0);

        printf("%sn", recvBuf);

        closesocket(sockConn);
    }

    WSACleanup();

    return 0;
}