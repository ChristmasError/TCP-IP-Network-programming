#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<WinSock2.h>
int main()
{
	//这里的代码都省略了异常处理
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//inet_addr()
	{
		const char* addr = "127.212.124.78";
		unsigned long conv_addr = inet_addr(addr);//0x4e7cd47f
		printf("%#lx\n", conv_addr);//Network ordered integer addr
	}
	//inet_ntoa()
	{
		struct sockaddr_in addr;
		char*strPtr;
		char strArr[20];

		addr.sin_addr.s_addr = htonl(0x1020304);
		strPtr = inet_ntoa(addr.sin_addr);
		strcpy_s(strArr, strPtr);
		printf("%s \n", strArr);//1.2.3.4
	}
	getchar();
	return 0;
}