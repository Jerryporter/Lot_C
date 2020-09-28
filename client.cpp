#include <winsock2.h>
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

    //第一步，创建套接字

    SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

    //定义套接字地址

    SOCKADDR_IN addrSrv;

    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //获取服务器IP地址,inet_addr()将IP地址转为点分十进制的格式

    addrSrv.sin_family = AF_INET;

    //sin_family 表示地址族，对于IP地址，sin_family成员将一直是AF_INET

    addrSrv.sin_port = htons(1234);

    //连接服务器

    //      connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

    if (connect(sockClient, (SOCKADDR *)&addrSrv, sizeof(SOCKADDR)) != 0)

    {

        //MessageBox("连接失败");

        //return;

        printf("error");

        return 0;
    }
    else

    {

        printf("success");
    }

    char recvBuf[100];

    recv(sockClient, recvBuf, 100, 0);

    printf("客户端接收到的数据：%s", recvBuf);

    send(sockClient, "client send ", strlen("client send ") + 1, 0);

    //关闭套接字

    closesocket(sockClient);

    //清除套接字资源

    WSACleanup();

    return 0;
}