#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
void ErrorHandling(const char* message);

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strlen;
	/*
	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}*/

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error!");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //localhost,回送地址，本地测试用
	servAddr.sin_port = htons(8888);	//服务器端口号

	//向服务器发送链接请求
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	//recv()接受服务器传来的信息
	strlen = recv(hSocket, message, sizeof(message) - 1, 0);
	if (strlen == -1)
		ErrorHandling("read() error!");
	
	printf("Message from server:%s\n", message);
	closesocket(hSocket);
	WSACleanup();
	getchar();	getchar();	getchar();
	return 0;
}

void ErrorHandling(const char * message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
