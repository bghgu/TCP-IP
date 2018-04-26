#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024
#define OPSZ 4

void ErrorHandling(char *message);

int main(int argc, char *argv[]) {

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAdr;

	char read_buf[BUF_SIZE];
	char opmsg[BUF_SIZE];

	int count = 1;
	int num = 0;
	int str_len = 0;

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
	else
		puts("숫자를 맞추시오.");

	
	while (true) {
		memset(&opmsg, 0, sizeof(opmsg));
		memset(read_buf, 0, sizeof(char) * 1024);

		printf("숫자를 입력하시오 : ");
		scanf("%d", &num);
		opmsg[0] = (char)num;
		send(hSocket, opmsg, 1, 0);

		memset(read_buf, 0, sizeof(char) * 1024);
		str_len = 0;

		recv(hSocket, (char*)(&str_len), 4, 0);
		if (str_len != 16) {
			break;
		}
		recv(hSocket, read_buf, str_len, 0);
		puts(read_buf);
		count++;
	}

	printf("%d번 만에 맞추었습니다. 클라이언트 종료합니다.\n", count);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}