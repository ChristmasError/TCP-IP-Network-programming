//以下均在VS2015 WINDOWS平台编译并运行
//在Linux中 头文件为：
//#incude<sys/socket.h>

//以下为服务端代码示例
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>
#define ser_port 8888 //端口号

void ErrorHandling(const char* message);
/*
argc为参数个数
argv[]为argc个参数，其中首个参数为程序的全名
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
	Winsock的初始化
	MAKEWORD(2,2)返回值为WORD，WORD为套接字的版本信息，这里为主版本为2，副版本为2，返回0x0202；
	第二个参数为WSADATA类型参数的地址；
	*/
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	//创建套接字
	hServerSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServerSock == INVALID_SOCKET)
		ErrorHandling("socket() error!");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(ser_port);

	//bind()为socker()函数创建的套接字分配IP地址和端口号
	if (bind(hServerSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error!");
	/*
	listen() 服务器等待连接数据 开始监听
	*/
	if (listen(hServerSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error!");

	/*
	accept()函数受理客户端连接请求
	*/
	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServerSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error!");

	/*
	send()向accept()连接的客户端传输数据
	*/
	send(hClntSock, message, sizeof(message), 0);


	closesocket(hClntSock);
	closesocket(hServerSock);
	//终止前注销WSAStarup()初始化的套接字库
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}