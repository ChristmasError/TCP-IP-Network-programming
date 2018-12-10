#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h> 
#include <stdio.h>
#include <vector>
#include<ws2tcpip.h>//定义socklen_t

using namespace std;


#pragma comment(lib, "WS2_32")  // 链接到WS2_32.lib


#define SERVER_IP "127.0.0.1"// 默认服务器端IP地址
#define SERVER_PORT 8888// 服务器端口号


class server
{
public:
	server();
	void init();
	void process();

private:
	int listener;//监听套接字
	sockaddr_in  serverAddr;//IPV4的地址方式
	vector <int> socksArr;//存放创建的套接字
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
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);//将字符串类型转换uint32_t
}
//初始化函数，创建监听套接字，绑定端口，并进行监听
void server::init()
{
	WSADATA   wsaData;                   
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	listener = socket(AF_INET, SOCK_STREAM, 0);//采用ipv4,TCP传输
	if (listener == -1) { printf("Error at socket(): %ld\n", WSAGetLastError()); perror("listener failed"); exit(1); }
	else printf("Server is running......\n");

	if (bind(listener, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror("bind error");
		exit(1);
	}
	listen(listener, 5);//listener这个套接字监听申请的链接，最大等待连接队列为5,等待accept()
	socksArr.push_back(listener);//将监听套接字加入套接字数组，数组内首个套接字就是服务器的套接字
}

void server::process()
{

	int mount = 0;
	fd_set fds,fds_copy;
	FD_ZERO(&fds);//将fds清零

	init();
	//下面就是不断的检查申请的连接队列
	printf("Server is waiting......\n");
	while (1)
	{
		mount = socksArr.size();
		//每次循环更新一次fds数组
		for (int i = 0; i<mount; ++i)
		{
			FD_SET(socksArr[i], &fds);
		}

		struct timeval timeout = { 1,0 };//每个Select等待三秒
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
		case 0:		//等待时间内无客户端申请连接
		{
			break;
		}
		default:
		{
			//将数组中的每一个套接字都和剩余的套接字进行比较，从而得到当前的任务
			for (int i = 0; i < mount; ++i)
			{
				//首个套接字就是服务器的套接字，当期存在并收到客户端连接请求，进行连接并更新套接字数组（将连接的客户端套接字加进数组中）
				if (i == 0 && FD_ISSET(socksArr[i], &fds))
				{
					struct sockaddr_in client_addr;
					int clntSz = sizeof(struct sockaddr_in);
					//返回一个用户的套接字
					int clientfd = accept(listener, (struct sockaddr*)&client_addr, &clntSz);
					//添加用户，服务器上显示消息，并通知用户连接成功
					socksArr.push_back(clientfd);
					printf("connect sucessfully\n");
					char ID[1024];
					sprintf(ID, "You ID is: %d and ", clientfd);
					char buf[30] = "welcome joint the chatroom! \n";
					strcat(ID, buf);
					send(clientfd, ID, sizeof(ID) - 1, 0);//减去最后一个'/0'
				}
				if (i != 0 && FD_ISSET(socksArr[i], &fds))
				{
					char buf[1024];
					memset(buf, '\0', sizeof(buf));
					int size = recv(socksArr[i], buf, sizeof(buf) - 1, 0);
					//检测是否断线
					if (size == 0 || size == -1)
					{
						printf("client %d leave\n", socksArr[i]);

						closesocket(socksArr[i]);//关闭这个套接字
						FD_CLR(socksArr[i], &fds);//在列表中删除
						socksArr.erase(socksArr.begin() + i);//在套接字数组中删除
					}
					//若是没有掉线
					else
					{
						printf("clint %d says: %s \n", socksArr[i], buf);
						//将此客户端说的话发送给每一个客户端
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