#include <winsock.h>
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <vector>
#include "mingw-threads/mingw.thread.h"
#include "nlohmann/json.hpp"
#pragma comment(lib, "wsock32.lib")

using json = nlohmann::json;
using std::cin;
using std::cout;
using std::endl;
using std::vector;

#define SERVER_PORT htons(8899)
#define SERVER_IP inet_addr("192.168.2.74")
#define SERVER_PORT_TEST htons(0x1234)
#define SERVER_IP_TEST htonl(INADDR_ANY)
