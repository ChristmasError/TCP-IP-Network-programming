#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>
#define ser_port 8888 //�˿ں�
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
	//receive.dat �Ƕ�д���ļ������к���Զ����ɣ������ü��±��鿴
	hSocket = socket(PF_INET,SOCK_STREAM,0);

	memset(&servAdr, 0, sizeof(&servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(ser_port);

	connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr));

	while ((readCnt = recv(hSocket, buf, BUF_SIZE, 0) != 0))
		fwrite((void*)buf, 1, readCnt, fp);

	puts("Received file data ");
	send(hSocket, "Thank you", 10, 0);//�����10�� "Thank you"�ĳ��ȣ���\0����Խ����
	fclose(fp);
	getchar();
	closesocket(hSocket);
	WSACleanup(); 
	return 0;
}