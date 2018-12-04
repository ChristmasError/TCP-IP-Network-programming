#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>
#define ser_port 8888
#define BUF_SIZE 1024

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main()
{
	WSADATA wsaData;
	SOCKET servSock;
	SOCKADDR_IN servAdr, clntAdr;
	char message[BUF_SIZE];
	int strLen=0;
	int clntAdrSz;
	//省略异常处理
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	servSock = socket(PF_INET, SOCK_DGRAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(ser_port);

	if(bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr))!=SOCKET_ERROR)
		printf_s("Server is running ...... \n");
	while (1)
	{
		clntAdrSz = sizeof(clntAdr);
		strLen = recvfrom(servSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAdr, &clntAdrSz);
		if (strLen != 0)
		{
			printf_s("accept!\n");
			sendto(servSock, message, strLen, 0, (SOCKADDR*)&clntAdr, sizeof(clntAdr));
	}
	closesocket(servSock);
	WSACleanup();
	return 0;
}