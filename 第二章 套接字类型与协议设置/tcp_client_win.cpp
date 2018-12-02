#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
//这是将静态库连接到工程中
//否则会出现一串无法解析的外部符号的LINK2019的报错
 void ErrorHandling(const char* message);

 int main()
 {
	 WSADATA wsaData;
	 //声明SOCKET变量保存socket()函数的返回值
	 //在Windows里sokect()的返回值是SOCKET
	 SOCKET hSocket;		
	 SOCKADDR_IN servAddr;
	 
	 char message[30];
	 int strLen = 0;
	 int readLen = 0;
	 int idx=0;
	 /*
	 if (argc != 3) {
	 printf("Usage: %s <IP> <port>\n", argv[0]);
	 exit(1);
	 }*/
	 if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		 ErrorHandling("WSAStartup() error!");

	 hSocket = socket(PF_INET, SOCK_STREAM, 0);
	 if (hSocket == INVALID_SOCKET)
		 ErrorHandling("socket() error!");

	 memset(&servAddr, 0, sizeof(servAddr));
	 servAddr.sin_family = AF_INET;
	 servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  
	 servAddr.sin_port = htons(8888);	
	 
	 if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		 ErrorHandling("connect() error!");
	//while中recv()读取数据，每次1字节
	//第一章中：
	//strlen = recv(hSocket, message, sizeof(message) - 1, 0);
	//每次读取的字符数是整个message的sizeof(message);
	 while (readLen = recv(hSocket, &message[idx++], 1, 0))
	 {
		 if(readLen==-1)
			 ErrorHandling("read() error!");
		 strLen += readLen;
		 if (message[idx - 1] == '\0')//字符串结束符跳出循环
			 break;
	}
	 std::cout << "Message from server : " << message << std::endl;
	 std::cout << "Function read call count : " << strLen << std::endl;

	 closesocket(hSocket);
	 WSACleanup();
	 getchar();
	 return 0;
 }

 void ErrorHandling(const char * message) {
	 fputs(message, stderr);
	 fputc('\n', stderr);
	 exit(1);
 }
