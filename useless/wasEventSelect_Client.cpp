// wasEventSelect 的客户端
//


#include <Winsock2.h>
#include <malloc.h>
#pragma comment(lib,"Ws2_32.lib")

SOCKET g_socketServer = INVALID_SOCKET;
WSAEVENT g_wsaEvent = WSA_INVALID_EVENT;

// 初始化socket
void InitSock()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return;
	}

	/* Confirm that the WinSock DLL supports 2.2.        */
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			WSACleanup( );
			return;
	}
}

void UnInitSock()
{
	if (g_wsaEvent != WSA_INVALID_EVENT)
	{
		WSACloseEvent(g_wsaEvent);
	}
	if (g_socketServer != INVALID_SOCKET)
	{
		closesocket(g_socketServer);
	}
	WSACleanup();
}

struct ST_THREAD_PARAM
{
	SOCKET socket;
	WSAEVENT wsaEvent;
};

DWORD WINAPI ServiceThread(LPVOID lpThreadParameter)
{
	ST_THREAD_PARAM* pThread = (ST_THREAD_PARAM*)lpThreadParameter;
	SOCKET socketServer = pThread->socket;
	WSAEVENT wsaEvent = pThread->wsaEvent;

	printf("新线程%d起动/n",GetCurrentThreadId());

	while(true)
	{
		int nRet=::WSAWaitForMultipleEvents(1,&wsaEvent,FALSE,10000,FALSE);
		if(nRet==WAIT_FAILED) // 失败
		{
			printf("failed WSAWaitForMultipleEvents/n");
			break;
		}
		else if(nRet==WSA_WAIT_TIMEOUT) // 超时
		{
			printf(" WSA_WAIT_TIMEOUT ... /n");
			continue;
		}
		else // 成功 -- 网络事件发生
		{
			WSANETWORKEVENTS wsaNetEvent;
			::WSAEnumNetworkEvents(socketServer,wsaEvent,&wsaNetEvent);
			if(wsaNetEvent.lNetworkEvents&FD_READ)
			{
				printf("FD_READ event occurs.../n");
				char buffer[1024];
				int nRet = recv(socketServer,buffer,1024,0);
				if (nRet>0)
				{
					buffer[nRet] = '/0';
					printf("收到数据 %s/n",buffer);
					Sleep(3000);
					printf("发送数据..../n");
					int nSend = send(socketServer,buffer,nRet,0);
				}
				else
				{
					UnInitSock();
					printf("线程%d退出/n",GetCurrentThreadId());
					return -1;
				}

			}
			else if(wsaNetEvent.lNetworkEvents&FD_WRITE)
			{
				printf("FD_WRITE event occurs.../n");
				printf("发送数据..../n");
				char szBuffer[] = "Hello world...";
				int nLen = sizeof(szBuffer);
				nRet = send(socketServer,szBuffer,nLen,0);
				if (nRet == SOCKET_ERROR)
				{
					printf("send() error .../n");
					UnInitSock();
					printf("线程%d退出/n",GetCurrentThreadId());
					return -1;
				}
				printf("nRet of send() is %d/n",nRet);
			}
			if(wsaNetEvent.lNetworkEvents&FD_CLOSE)
			{
				printf("FD_CLOSE event occurs.../n");
				int nErrorCode = WSAGetLastError();
				printf("Error code is %d/n",nErrorCode);
				if (nErrorCode == WSAECONNRESET)
				{
					printf("WSAECONNRESET error./n");
				}
				else if (nErrorCode == WSAENETDOWN)
				{
					printf("WSAENETDOWN error./n");
				}
				else if (nErrorCode == WSAENETRESET)
				{
					printf("WSAENETRESET error./n");
				}
				UnInitSock();
				printf("线程%d退出/n",GetCurrentThreadId());
				return -1;
			}
		}
	}

	printf("线程%d退出/n",GetCurrentThreadId());

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	InitSock();
	g_socketServer=socket(AF_INET,SOCK_STREAM,0);
	if (g_socketServer == INVALID_SOCKET)
	{
		UnInitSock();
		return -1;
	}
	sockaddr_in sin;
	sin.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
	sin.sin_family=AF_INET;
	sin.sin_port=htons(3456);
	if (connect(g_socketServer,(sockaddr*)&sin,sizeof(sockaddr)) == SOCKET_ERROR)
	{
		UnInitSock();
		return -1;
	}
	g_wsaEvent=::WSACreateEvent();
	::WSAEventSelect(g_socketServer,g_wsaEvent,FD_READ|FD_WRITE|FD_CLOSE);

	ST_THREAD_PARAM* pThreadParam = new ST_THREAD_PARAM();
	pThreadParam->socket = g_socketServer;
	pThreadParam->wsaEvent = g_wsaEvent;
	HANDLE hThread = ::CreateThread(NULL,0,ServiceThread,(LPVOID)pThreadParam,0,NULL);
	if (hThread == INVALID_HANDLE_VALUE)
	{
		printf("Failed to create new thread.../n");
		UnInitSock();
		return -1;
	}

	printf("WaitForSingleObject(hThread,INFINITE) waiting.../n");
	WaitForSingleObject(hThread,INFINITE);
	printf("WaitForSingleObject(hThread,INFINITE) return .../n");
	delete pThreadParam;

	//while(true)
	//{
	//	int nRet=::WSAWaitForMultipleEvents(1,&g_wsaEvent,FALSE,10000,FALSE);
	//	if(nRet==WAIT_FAILED) // 失败
	//	{
	//		printf("failed wait for single object/n");
	//		break;
	//	}
	//	else if(nRet==WSA_WAIT_TIMEOUT) // 超时
	//	{
	//		printf(" WSA_WAIT_TIMEOUT ... /n");
	//		continue;
	//	}
	//	else // 成功 -- 网络事件发生
	//	{
	//		WSANETWORKEVENTS wsaNetEvent;
	//		::WSAEnumNetworkEvents(g_socketServer,g_wsaEvent,&wsaNetEvent);
	//		if(wsaNetEvent.lNetworkEvents&FD_READ)
	//		{
	//			printf("FD_READ event occurs.../n");
	//			char buffer[1024];
	//			int nRet = recv(g_socketServer,buffer,1024,0);
	//			if (nRet>0)
	//			{
	//				buffer[nRet] = '/0';
	//				printf("收到数据 %s/n",buffer);
	//				Sleep(3000);
	//				printf("发送数据..../n");
	//				int nSend = send(g_socketServer,buffer,nRet,0);
	//			}
	//			else
	//			{
	//				UnInitSock();
	//				return -1;
	//			}
	//
	//		}
	//		else if(wsaNetEvent.lNetworkEvents&FD_WRITE)
	//		{
	//			printf("FD_WRITE event occurs.../n");
	//			printf("发送数据..../n");
	//			char szBuffer[] = "Hello world...";
	//			int nLen = sizeof(szBuffer);
	//			nRet = send(g_socketServer,szBuffer,nLen,0);
	//			if (nRet == SOCKET_ERROR)
	//			{
	//				printf("send() error .../n");
	//				UnInitSock();
	//				return -1;
	//			}
	//			printf("nRet of send() is %d/n",nRet);
	//		}
	//		if(wsaNetEvent.lNetworkEvents&FD_CLOSE)
	//		{
	//			printf("FD_CLOSE event occurs.../n");
	//			int nErrorCode = WSAGetLastError();
	//			printf("Error code is %d/n",nErrorCode);
	//			if (nErrorCode == WSAECONNRESET)
	//			{
	//				printf("WSAECONNRESET error./n");
	//			}
	//			else if (nErrorCode == WSAENETDOWN)
	//			{
	//				printf("WSAENETDOWN error./n");
	//			}
	//			else if (nErrorCode == WSAENETRESET)
	//			{
	//				printf("WSAENETRESET error./n");
	//			}
	//			UnInitSock();
	//			return -1;
	//		}
	//	}
	//}

	return 0;
}

/*
* Windows Sockets definitions of regular Berkeley error constants
#define WSAEWOULDBLOCK          (WSABASEERR+35)
#define WSAEINPROGRESS          (WSABASEERR+36)
#define WSAEALREADY             (WSABASEERR+37)
#define WSAENOTSOCK             (WSABASEERR+38)
#define WSAEDESTADDRREQ         (WSABASEERR+39)
#define WSAEMSGSIZE             (WSABASEERR+40)
#define WSAEPROTOTYPE           (WSABASEERR+41)
#define WSAENOPROTOOPT          (WSABASEERR+42)
#define WSAEPROTONOSUPPORT      (WSABASEERR+43)
#define WSAESOCKTNOSUPPORT      (WSABASEERR+44)
#define WSAEOPNOTSUPP           (WSABASEERR+45)
#define WSAEPFNOSUPPORT         (WSABASEERR+46)
#define WSAEAFNOSUPPORT         (WSABASEERR+47)
#define WSAEADDRINUSE           (WSABASEERR+48)
#define WSAEADDRNOTAVAIL        (WSABASEERR+49)
#define WSAENETDOWN             (WSABASEERR+50)
#define WSAENETUNREACH          (WSABASEERR+51)
#define WSAENETRESET            (WSABASEERR+52)
#define WSAECONNABORTED         (WSABASEERR+53)
#define WSAECONNRESET           (WSABASEERR+54)
#define WSAENOBUFS              (WSABASEERR+55)
#define WSAEISCONN              (WSABASEERR+56)
#define WSAENOTCONN             (WSABASEERR+57)
#define WSAESHUTDOWN            (WSABASEERR+58)
#define WSAETOOMANYREFS         (WSABASEERR+59)
#define WSAETIMEDOUT            (WSABASEERR+60)
#define WSAECONNREFUSED         (WSABASEERR+61)
#define WSAELOOP                (WSABASEERR+62)
#define WSAENAMETOOLONG         (WSABASEERR+63)
#define WSAEHOSTDOWN            (WSABASEERR+64)
#define WSAEHOSTUNREACH         (WSABASEERR+65)
#define WSAENOTEMPTY            (WSABASEERR+66)
#define WSAEPROCLIM             (WSABASEERR+67)
#define WSAEUSERS               (WSABASEERR+68)
#define WSAEDQUOT               (WSABASEERR+69)
#define WSAESTALE               (WSABASEERR+70)
#define WSAEREMOTE              (WSABASEERR+71)
*/