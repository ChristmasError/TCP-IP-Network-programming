#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <WinSock2.h>
#define BUF_SIZE 1024
#define ser_port 8888 
#define OPSZ 4
#define RLT_SIZE 4
void ErrorHandling(const char * message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


int main() {
	WSADATA wsaData;
	SOCKET hSocket;
	char opmsg[BUF_SIZE];
	int result, opndCnt, i;
	SOCKADDR_IN servAdr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error!");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(ser_port);

	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error!");
		getchar();
	}
	else
		puts("Connected......");

	fputs("Operand count : " ,stdout);
	scanf_s("%d", &opndCnt);
	opmsg[0] = (char)opndCnt;
	
	for ( i = 0; i < opndCnt; i++)
	{
		printf("Operand %d : ", i + 1);
		scanf_s("%d", (int*)&opmsg[ i * OPSZ + 1]);
	}
	fgetc(stdin);
	fputs("Operator : ", stdout);
	scanf_s("%c", &opmsg[opndCnt*OPSZ + 1]);
	send(hSocket, opmsg, opndCnt*OPSZ + 2, 0);
	recv(hSocket, (char*)&result, RLT_SIZE, 0);

	printf("Operator result :%d \n", result);

	closesocket(hSocket);	
	getchar(); getchar();
	WSACleanup();
	return 0;
}
