#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>
#define ser_port 8888 //端口号
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
	SOCKET hSocket;
	SOCKADDR_IN servAdr;
	int readCnt;
	FILE * fp;
	char buf[BUF_SIZE];
	
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	fp = fopen("receive.dat", "wb");
	//receive.dat 是读写的文件，运行后会自动生成，可以用记事本查看
	hSocket = socket(PF_INET,SOCK_STREAM,0);

	memset(&servAdr, 0, sizeof(&servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(ser_port);

	connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr));

	while ((readCnt = recv(hSocket, buf, BUF_SIZE, 0) != 0))
		fwrite((void*)buf, 1, readCnt, fp);

	puts("Received file data ");
	send(hSocket, "Thank you", 10, 0);//这里的10是 "Thank you"的长度，含\0，别越界了
	fclose(fp);
	getchar();
	closesocket(hSocket);
	WSACleanup(); 
	return 0;
}