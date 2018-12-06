#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>
#include<iostream>
#define ser_port 8888 //端口号
#define BUF_SIZE 1024
#define OPSZ 4
using namespace std;
void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main()
{
	WSADATA wsaData;
	SOCKET hAcptSock, hRecvSock;
	SOCKADDR_IN recvAdr, sendAdr;
	TIMEVAL timeout;
	fd_set reads, except ,readCopy,exceptCopy;
	//except存放接收的紧急数据的信息

	int sendAdrSize;
	int result ,strLen;
	char buf[BUF_SIZE];

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	hAcptSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&recvAdr, 0, sizeof(&recvAdr));
	recvAdr.sin_family = AF_INET;
	recvAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	recvAdr.sin_port = htons(ser_port);

	bind(hAcptSock, (SOCKADDR*)&recvAdr, sizeof(recvAdr));
	listen(hAcptSock, 5);

	sendAdrSize = sizeof(sendAdr);
	hRecvSock = accept(hAcptSock, (SOCKADDR*)&sendAdr, &sendAdrSize);
	
	FD_ZERO(&reads);
	FD_ZERO(&except);
	FD_SET(hRecvSock, &reads);
	FD_SET(hRecvSock, &except);

	while (1)
	{
		readCopy = reads;
		exceptCopy = except;

		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		result = select(0, &readCopy, 0, &exceptCopy, &timeout);


		if (result > 0)
		{
			if (FD_ISSET(hRecvSock, &exceptCopy))//先处理紧急消息
			{
				strLen = recv(hRecvSock, buf, BUF_SIZE - 1, MSG_OOB);
				buf[strLen] = 0;
				printf("Urgent message : %s \n", buf);
			}
			if (FD_ISSET(hRecvSock, &readCopy))//后处理正常消息
			{
				strLen = recv(hRecvSock, buf, BUF_SIZE - 1, 0);
				if (strLen == 0) {
					break;
					closesocket(hRecvSock);
				}
				else
				{
					buf[strLen] = 0;
					puts(buf);
				}
			}
		}
	}
	closesocket(hAcptSock);
	WSACleanup();
	return 0;
}