#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#define BUF_SIZE 1024
#define ser_port 8888 
void ErrorHandling(const char * message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


int main() {
	WSADATA wsaData;
	SOCKET hSocket;
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
	int i = 0;
	//while (i++<10) {
	//请在这打断点并一步步运行，模拟数据的逐行发送
		send(hSocket, "123" , 3, 0);
		send(hSocket, "4", 1, MSG_OOB);
		send(hSocket, "567",3, 0);
		send(hSocket, "890", 3, MSG_OOB);
	//}
	closesocket(hSocket);
	WSACleanup();
	return 0;
}
