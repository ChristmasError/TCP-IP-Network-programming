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
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	TIMEVAL timeout;
	fd_set reads, cpyReads;

	int adrSz;
	int strLen, fdNum, i;
	char buf[BUF_SIZE];

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAdr, 0, sizeof(&servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(ser_port);

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	listen(hServSock, 5);

	FD_ZERO(&reads);
	//fd_set位置初始化0
	FD_SET(hServSock, &reads);
	//給fd_set变量reads上注册服务器端套接字
	while (1)
	{
		cpyReads = reads;
		//将初始化好的reads复制给cpyReads
		//因为调用select后，除了发生变化的套接字的在FD_SET对应位变化外
		//剩下所有应初始化为0，为了记住初始值，必须经历复制过程
		//这是select()通用方法，无论WINDOWS还是Linux
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;
		//每次调用select()前都初始化timeval结构新值
		//timeval结构保存（time—out）超时信息，防止陷入无限阻塞状态
		fdNum = select(0, &cpyReads, 0, 0, &timeout);
		//调用select
		//在windows内，第一个参数只为了保持与其他操作系统兼容而添加，并无特殊意义

		if (fdNum == 0)
			continue;

		for (i = 0; i < reads.fd_count; i++)
		{
			if (FD_ISSET(reads.fd_array[i], &cpyReads))
			{
				if (reads.fd_array[i] == hServSock)  //连接请求
				{
					adrSz = sizeof(clntAdr);
					hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &adrSz);
					FD_SET(hClntSock, &reads);//将客户端套接字注册进fd_set中
					printf("connected clent %d : \n",hClntSock);
				}
				else //读取信息数据
				{
					strLen = recv(reads.fd_array[i], buf, BUF_SIZE - 1, 0);
					if (strLen == 0)	//当没有数据传输，并将fd_set对应位的数据清除
					{
						FD_CLR(reads.fd_array[i], &reads);
						closesocket(cpyReads.fd_array[i]);
						printf("Closed clent : %d \n", cpyReads.fd_array[i]);
					}
					else //有数据传输的情况
					{
						send(reads.fd_array[i], buf, strLen, 0);		//回声
					}
				}
			}
		}
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}