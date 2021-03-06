#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void ErrorHandling(char *message);

int main(int argc, char *argv[]) {

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAdr;

	char read_buf[BUF_SIZE];
	char helloMsg[] = "Hello Server!";
	char IntroMsg[] = "I`m Client.";
	char NiceMsg[] = "Nice to meet you, too!";
	char* str_arr[] = { helloMsg , IntroMsg, NiceMsg };

	int str_len;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");

	for (int i = 0; i < 3; i++) {
		recv(hSocket, (char*)(&str_len), 4, 0);
		recv(hSocket, read_buf, str_len, 0);

		puts(read_buf);

		//종료 문자 때문에 +1
		str_len = strlen(str_arr[i]) + 1;

		send(hSocket, (char*)(&str_len), 4, 0);
		send(hSocket, str_arr[i], str_len, 0);
	}

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}