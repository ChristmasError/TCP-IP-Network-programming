#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<WinSock2.h>
#define BUF_SIZE 1024
#define ser_port 8888
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
	SOCKET sock;
	char message[BUF_SIZE];
	int strLen,clntadr;
	SOCKADDR_IN servAdr,client_adr;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sock = socket(PF_INET, SOCK_DGRAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//cout << inet_addr("127.0.0.1") << endl;
	//cout << inet_addr("127,0,0,1") << endl; 小心注意，不要问我为什么加这条注释
	servAdr.sin_port = htons(ser_port);
	connect(sock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	while (1)
	{
		std::cout << "Insert message (q to quite) : ";
		fgets(message, sizeof(message), stdin);
		//cin.getline(message, sizeof(message));
		if (!strcmp(message, "q\n"))
			break;
		int clntAdrSz = sizeof(client_adr);
		send(sock, message, strlen(message), 0);
		strLen = recv(sock, message, sizeof(message) - 1 , 0); 
		/*
		if (strLen = recvfrom(sock, message, BUF_SIZE, 0, (SOCKADDR*)&client_adr, &clntAdrSz) == SOCKET_ERROR)
		{
			printf("err");
		}*/
		message[strLen] = 0;
		//cout << "Message from server :" << message<<endl;
		printf("Message from serve : %s ", message);
	}
	getchar(); getchar(); getchar();
	closesocket(sock);
	WSACleanup();
	return 0;
}