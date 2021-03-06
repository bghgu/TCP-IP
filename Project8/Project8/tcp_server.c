#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024
#define OPSZ 4

void ErrorHandling(char *message);

int main(int argc, char *argv[]) {

	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;

	char read_buf[100];
	char helloMsg[] = "Hello Client!";
	char IntroMsg[] = "I`m Server.";
	char NiceMsg[] = "Nice to meet you!";
	char* str_arr[] = { helloMsg, IntroMsg, NiceMsg };

	int str_len, clntAdrSize;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");
		
	clntAdrSize = sizeof(clntAdr);
	hClntSock = accept(hServSock, (struct sockaddr*)&clntAdr, &clntAdrSize);
	if (hClntSock == -1)
		ErrorHandling("accept() error");

	for (int i = 0; i < 3; i++) {
		//종료 문자 때문에 +1
		str_len = strlen(str_arr[i]) + 1;
		
		send(hClntSock, (char*)(&str_len), 4, 0);
		send(hClntSock, str_arr[i], str_len, 0);

		recv(hClntSock, (char*)(&str_len), 4, 0);
		recv(hClntSock, read_buf, str_len, 0);
		puts(read_buf);
	}

	closesocket(hServSock);
	closesocket(hClntSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}