//���¾���VS2015 WINDOWSƽ̨���벢����
//��Linux�� ͷ�ļ�Ϊ��
//#incude<sys/socket.h>

//����Ϊ����˴���ʾ��
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>
#define ser_port 8888 //�˿ں�

void ErrorHandling(const char* message);
/*
argcΪ��������
argv[]Ϊargc�������������׸�����Ϊ�����ȫ��
*/
int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServerSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	argc = 1;
	int szClntAddr;
	char message[] = "Hello World!";
	/*
	if (argc != 2)
	{
	printf("Usage :%s <port>\n", argv[0]);
	exit(1);
	}*/
	/*
	Winsock�ĳ�ʼ��
	MAKEWORD(2,2)����ֵΪWORD��WORDΪ�׽��ֵİ汾��Ϣ������Ϊ���汾Ϊ2�����汾Ϊ2������0x0202��
	�ڶ�������ΪWSADATA���Ͳ����ĵ�ַ��
	*/
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	//�����׽���
	hServerSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServerSock == INVALID_SOCKET)
		ErrorHandling("socket() error!");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(ser_port);

	//bind()Ϊsocker()�����������׽��ַ���IP��ַ�Ͷ˿ں�
	if (bind(hServerSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error!");
	/*
	listen() �������ȴ��������� ��ʼ����
	*/
	if (listen(hServerSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error!");

	/*
	accept()��������ͻ�����������
	*/
	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServerSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error!");

	/*
	send()��accept()���ӵĿͻ��˴�������
	*/
	send(hClntSock, message, sizeof(message), 0);


	closesocket(hClntSock);
	closesocket(hServerSock);
	//��ֹǰע��WSAStarup()��ʼ�����׽��ֿ�
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}