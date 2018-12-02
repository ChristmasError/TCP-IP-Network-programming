#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
//���ǽ���̬�����ӵ�������
//��������һ���޷��������ⲿ���ŵ�LINK2019�ı���
 void ErrorHandling(const char* message);

 int main()
 {
	 WSADATA wsaData;
	 //����SOCKET��������socket()�����ķ���ֵ
	 //��Windows��sokect()�ķ���ֵ��SOCKET
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
	//while��recv()��ȡ���ݣ�ÿ��1�ֽ�
	//��һ���У�
	//strlen = recv(hSocket, message, sizeof(message) - 1, 0);
	//ÿ�ζ�ȡ���ַ���������message��sizeof(message);
	 while (readLen = recv(hSocket, &message[idx++], 1, 0))
	 {
		 if(readLen==-1)
			 ErrorHandling("read() error!");
		 strLen += readLen;
		 if (message[idx - 1] == '\0')//�ַ�������������ѭ��
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
