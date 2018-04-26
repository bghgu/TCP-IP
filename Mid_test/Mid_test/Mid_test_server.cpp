#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>

#define BUF_SIZE 1024
#define OPSZ 4

void ErrorHandling(char *message);

int main(int argc, char *argv[]) {

	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;

	char msgbig[] = "그것 보단 크다.";
	char msgsmall[] = "그것 보단 작다.";

	srand((unsigned)time(NULL));
	int random = rand() % 101;

	int str_len;
	int clntAdrSize = 0;
	int num = 0;
	int count = 1;

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
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
	if (hClntSock == -1)
		ErrorHandling("accept() error");
	else {
		printf("연결 성공...\n");
		printf("난수 생성 : %d\n", random);
	}

	while (true) {
		num = 0;
		recv(hClntSock, (char*)&num, 1, 0);
		if (num == random) {
			printf("정답. 서버 종료합니다.\n");
			send(hClntSock, (char*)&count, sizeof(count), 0);
			break;
		}
		else {
			count++;
			str_len = 0;
			if (num < random) {
				str_len = strlen(msgbig) + 1;
				send(hClntSock, (char*)(&str_len), 4, 0);
				send(hClntSock, msgbig, str_len, 0);
			}
			else if(num > random) {
				str_len = strlen(msgsmall) + 1;
				send(hClntSock, (char*)(&str_len), 4, 0);
				send(hClntSock, msgsmall, str_len, 0);
			}
		}
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