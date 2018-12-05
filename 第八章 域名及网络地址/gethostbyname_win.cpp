#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main()
{
	WSADATA wsaData;
	int i;
	struct hostent *host;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	host = gethostbyname("www.baidu.com");

	printf("Official name : %s \n", host->h_name);
	for (int i = 0; host->h_aliases[i]; i++)
		printf("Aliases %d : %s \n", i + 1, host->h_aliases);

	printf("Address type: %s \n", host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6");;

	for (int i = 0; host->h_addr_list[i]; i++)
		printf("IP addr %d : %s \n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

	getchar();
	WSACleanup();
	return 0;
}