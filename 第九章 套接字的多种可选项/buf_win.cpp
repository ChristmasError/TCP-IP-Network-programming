#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<WinSock2.h>
using namespace std;

void showSocketBufSize(SOCKET sock) {
	int sndBuf, rcvBuf, state, len;

	len = sizeof(sndBuf);
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&sndBuf, &len);

	len = sizeof(rcvBuf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBuf, &len);

	cout << "Input buffer size £º" << rcvBuf << endl;
	cout << "Output buffer size : " << sndBuf << endl;
}

int main()
{
	WSADATA wsaData;
	SOCKET hSock;
	int sndBuf, rcvBuf, state;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	hSock = socket(PF_INET, SOCK_STREAM, 0);

	showSocketBufSize(hSock);

	sndBuf = 1024 * 3, rcvBuf = 1024 * 3;
	state = setsockopt(hSock, SOL_SOCKET, SO_SNDBUF, (char*)&sndBuf, sizeof(sndBuf));
	state = setsockopt(hSock, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBuf, sizeof(rcvBuf));
	showSocketBufSize(hSock);
	getchar();
	closesocket(hSock);
	WSACleanup();
	return 0;
}