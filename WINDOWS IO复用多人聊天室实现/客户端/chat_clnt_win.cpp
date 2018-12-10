#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<conio.h>
#include <iostream>
#include <thread>
#include <winsock2.h> 
#include <stdio.h>
#include<ws2tcpip.h>//����socklen_t

#pragma comment(lib, "WS2_32")  // ���ӵ�WS2_32.lib

using namespace std;



#define SERVER_IP "127.0.0.1"// Ĭ�Ϸ�������IP��ַ
#define SERVER_PORT 8888// �������˿ں�



class client
{
public:
	client();
	void init();
	void process();
private:
	int user; //�ͻ����׽��֣������
	int writing;
	sockaddr_in  serverAddr;
	void sendata();
};
int main()
{
	client user;
	user.process();

	return 0;
}

client::client()
{
	user = 0;
	writing = 0;
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = SERVER_PORT;
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);//���ַ�������ת��uint32_t
}

void client::init()
{
	WSADATA   wsaData;                											
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	user = socket(AF_INET, SOCK_STREAM, 0);//����ipv4,TCP���䣬�ɹ�ʱ���طǸ���socket������

	if (user <= 0)
	{
		perror("establish client faild");
		printf("Error at socket(): %ld\n", WSAGetLastError());
		exit(1);
	};
	printf("establish succesfully\n");//�����ɹ�������ʽ�ĵȴ�����������
	if (connect(user, (const sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror("connect to server faild");
		printf("Error at socket(): %ld\n", WSAGetLastError());
		exit(1);
	}
	printf("connect IP:%s  Port:%d succesfully\n", SERVER_IP, SERVER_PORT);//�����ɹ�
}

void client::process()
{
	char recvbuf[1024];
	fd_set fdread, fedwrite;
	FD_ZERO(&fdread);//��fdread����
	FD_ZERO(&fedwrite);//��fedwrite����

	init();

	while (1)
	{
		FD_SET(user, &fdread);
		if (writing == 0)
			FD_SET(user, &fedwrite);
		struct timeval timeout = { 1,0 };//ÿ��Select�ȴ�����
		switch (select(0, &fdread, &fedwrite, NULL, &timeout))
		{
		case -1:
		{
			printf("Error at socket(): %ld\n", WSAGetLastError());
			break;
		}
		case 0:		
			break;
		default:
		{
			if (FD_ISSET(user, &fdread))//�д����¼�
			{
				int size = recv(user, recvbuf, sizeof(recvbuf) - 1, 0);
				if (size > 0)
				{
					printf("server : %s\n", recvbuf);
					memset(recvbuf, '\0', sizeof(recvbuf));
				}
				else if (size == 0)
				{
					printf("server is closed\n");
					exit(1);
				}
			}
			if (FD_ISSET(user, &fedwrite))
			{
				FD_ZERO(&fedwrite);//��fedwrite����
				writing = 1;//��ʾ����д��
				thread sendtask(bind(&client::sendata, this));
				sendtask.detach();//�����̺߳������̷����һ��಻Ӱ��
			}
			break;
		}
	}
	}
}

void client::sendata()
{
	char sendbuf[1024];
	char middle[1024];

	cin.getline(sendbuf, 1024);//��ȡһ��
	send(user, sendbuf, sizeof(sendbuf) - 1, 0);
	writing = 0;
}