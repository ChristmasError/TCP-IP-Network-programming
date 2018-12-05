#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>
#define ser_port 8888 //¶Ë¿ÚºÅ
#define BUF_SIZE 30
#define OPSZ 4
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
	FILE *fp;
	char buf[BUF_SIZE];
	int readcnt;
	int clntAdrsz;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	fp = fopen("file_server_win.cpp", "rb");
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAdr, 0, sizeof(&servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(ser_port);

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	printf("server is running......\n");
	listen(hServSock, 5);

	clntAdrsz = sizeof(clntAdr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrsz);

	while(1)
	{
		readcnt = fread((void*)buf, 1, BUF_SIZE, fp);
		if (readcnt < BUF_SIZE)
		{
			send(hClntSock, (char*)&buf, readcnt, 0);
			break;
		}
		send(hClntSock, (char*)buf, BUF_SIZE, 0);
	}
	shutdown(hClntSock, SD_SEND);
	recv(hClntSock, (char*)&buf, BUF_SIZE, 0);
	printf("message from clent : %s \n", buf);

	fclose(fp);
	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();
	return 0;
}