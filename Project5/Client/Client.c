#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen;
	char buff[100];

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");

	strLen = recv(hSocket, message, sizeof(message) - 1, 0);
	if (strLen == -1)
		ErrorHandling("read() error!");
	printf("Message from server: %s \n", message);

	while (1) {
		//인풋 버퍼 초기화 해 주는 것이 좋다.
		memset(buff, 0, sizeof(char) * 100);
		printf("Type message to server : ");
		//scanf("%s", buff);
		fgets(buff, sizeof(buff), stdin);
		buff[strlen(buff) - 1] = '\0';
		send(hSocket, buff, sizeof(buff), 0);
		printf("Send an input message to server...!!!\n");
		if (strstr(buff, "bye") != NULL) break;
	}
	
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}