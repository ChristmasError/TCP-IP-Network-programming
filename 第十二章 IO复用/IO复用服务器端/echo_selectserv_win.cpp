#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>
#include<iostream>
#define ser_port 8888 //�˿ں�
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
	//fd_setλ�ó�ʼ��0
	FD_SET(hServSock, &reads);
	//�ofd_set����reads��ע����������׽���
	while (1)
	{
		cpyReads = reads;
		//����ʼ���õ�reads���Ƹ�cpyReads
		//��Ϊ����select�󣬳��˷����仯���׽��ֵ���FD_SET��Ӧλ�仯��
		//ʣ������Ӧ��ʼ��Ϊ0��Ϊ�˼�ס��ʼֵ�����뾭�����ƹ���
		//����select()ͨ�÷���������WINDOWS����Linux
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;
		//ÿ�ε���select()ǰ����ʼ��timeval�ṹ��ֵ
		//timeval�ṹ���棨time��out����ʱ��Ϣ����ֹ������������״̬
		fdNum = select(0, &cpyReads, 0, 0, &timeout);
		//����select
		//��windows�ڣ���һ������ֻΪ�˱�������������ϵͳ���ݶ���ӣ�������������

		if (fdNum == 0)
			continue;

		for (i = 0; i < reads.fd_count; i++)
		{
			if (FD_ISSET(reads.fd_array[i], &cpyReads))
			{
				if (reads.fd_array[i] == hServSock)  //��������
				{
					adrSz = sizeof(clntAdr);
					hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &adrSz);
					FD_SET(hClntSock, &reads);//���ͻ����׽���ע���fd_set��
					printf("connected clent %d : \n",hClntSock);
				}
				else //��ȡ��Ϣ����
				{
					strLen = recv(reads.fd_array[i], buf, BUF_SIZE - 1, 0);
					if (strLen == 0)	//��û�����ݴ��䣬����fd_set��Ӧλ���������
					{
						FD_CLR(reads.fd_array[i], &reads);
						closesocket(cpyReads.fd_array[i]);
						printf("Closed clent : %d \n", cpyReads.fd_array[i]);
					}
					else //�����ݴ�������
					{
						send(reads.fd_array[i], buf, strLen, 0);		//����
					}
				}
			}
		}
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}