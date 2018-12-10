#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h> 
#include <stdio.h>
#include <vector>
#include<ws2tcpip.h>//����socklen_t

using namespace std;


#pragma comment(lib, "WS2_32")  // ���ӵ�WS2_32.lib


#define SERVER_IP "127.0.0.1"// Ĭ�Ϸ�������IP��ַ
#define SERVER_PORT 8888// �������˿ں�


class server
{
public:
	server();
	void init();
	void process();

private:
	int listener;//�����׽���
	sockaddr_in  serverAddr;//IPV4�ĵ�ַ��ʽ
	vector <int> socksArr;//��Ŵ������׽���
};
int main()
{
	server ser;
	ser.process();

	return 0;
}


server::server()
{
	listener = 0;
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = SERVER_PORT;
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);//���ַ�������ת��uint32_t
}
//��ʼ�����������������׽��֣��󶨶˿ڣ������м���
void server::init()
{
	WSADATA   wsaData;                   
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	listener = socket(AF_INET, SOCK_STREAM, 0);//����ipv4,TCP����
	if (listener == -1) { printf("Error at socket(): %ld\n", WSAGetLastError()); perror("listener failed"); exit(1); }
	else printf("Server is running......\n");

	if (bind(listener, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror("bind error");
		exit(1);
	}
	listen(listener, 5);//listener����׽��ּ�����������ӣ����ȴ����Ӷ���Ϊ5,�ȴ�accept()
	socksArr.push_back(listener);//�������׽��ּ����׽������飬�������׸��׽��־��Ƿ��������׽���
}

void server::process()
{

	int mount = 0;
	fd_set fds,fds_copy;
	FD_ZERO(&fds);//��fds����

	init();
	//������ǲ��ϵļ����������Ӷ���
	printf("Server is waiting......\n");
	while (1)
	{
		mount = socksArr.size();
		//ÿ��ѭ������һ��fds����
		for (int i = 0; i<mount; ++i)
		{
			FD_SET(socksArr[i], &fds);
		}

		struct timeval timeout = { 1,0 };//ÿ��Select�ȴ�����
		switch (select(0, &fds, NULL, NULL, &timeout))
		{
		case -1:     //select error
		{
			perror("select\n");
			printf("Error at socket(): %ld\n", WSAGetLastError());
			printf("%d\n", mount);
			Sleep(1000);
			break;
		}
		case 0:		//�ȴ�ʱ�����޿ͻ�����������
		{
			break;
		}
		default:
		{
			//�������е�ÿһ���׽��ֶ���ʣ����׽��ֽ��бȽϣ��Ӷ��õ���ǰ������
			for (int i = 0; i < mount; ++i)
			{
				//�׸��׽��־��Ƿ��������׽��֣����ڴ��ڲ��յ��ͻ����������󣬽������Ӳ������׽������飨�����ӵĿͻ����׽��ּӽ������У�
				if (i == 0 && FD_ISSET(socksArr[i], &fds))
				{
					struct sockaddr_in client_addr;
					int clntSz = sizeof(struct sockaddr_in);
					//����һ���û����׽���
					int clientfd = accept(listener, (struct sockaddr*)&client_addr, &clntSz);
					//����û�������������ʾ��Ϣ����֪ͨ�û����ӳɹ�
					socksArr.push_back(clientfd);
					printf("connect sucessfully\n");
					char ID[1024];
					sprintf(ID, "You ID is: %d and ", clientfd);
					char buf[30] = "welcome joint the chatroom! \n";
					strcat(ID, buf);
					send(clientfd, ID, sizeof(ID) - 1, 0);//��ȥ���һ��'/0'
				}
				if (i != 0 && FD_ISSET(socksArr[i], &fds))
				{
					char buf[1024];
					memset(buf, '\0', sizeof(buf));
					int size = recv(socksArr[i], buf, sizeof(buf) - 1, 0);
					//����Ƿ����
					if (size == 0 || size == -1)
					{
						printf("client %d leave\n", socksArr[i]);

						closesocket(socksArr[i]);//�ر�����׽���
						FD_CLR(socksArr[i], &fds);//���б���ɾ��
						socksArr.erase(socksArr.begin() + i);//���׽���������ɾ��
					}
					//����û�е���
					else
					{
						printf("clint %d says: %s \n", socksArr[i], buf);
						//���˿ͻ���˵�Ļ����͸�ÿһ���ͻ���
						char client[1024];
						sprintf(client, "client %d:", socksArr[i]);
						strcat(client, buf);
						for (int j = 1; j < mount; j++)
						{
							send(socksArr[j], client, sizeof(client) - 1, 0);
						}
					}
				}
			}
			break;
		}
		}
	}
}