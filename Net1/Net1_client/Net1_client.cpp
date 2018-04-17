/*
	작성자 : 배다슬
	학번 : 201232016
	학과 : 소프트웨어공학과
	설명 : 계산기_클라이언트
*/

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
	char opmsg[BUF_SIZE];
	int result, opndCnt, i;

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
		puts("Connected...........");

	fputs("피 연산자 수: ", stdout);
	scanf("%d", &opndCnt);

	opmsg[0] = (char)opndCnt;

	for (i = 0; i<opndCnt; i++) {
		printf("%d 번째 피 연산자 : ", i + 1);
		//4바이트라 4칸씩 이동
		scanf("%d", (int*)&opmsg[i*OPSZ + 1]);
	}

	fgetc(stdin);
	fputs("연산자: ", stdout);
	scanf("%c", &opmsg[opndCnt*OPSZ + 1]);
	send(hSocket, opmsg, opndCnt*OPSZ + 2, 0);

	recv(hSocket, (char*)&result, RLT_SIZE, 0);

	printf("연산 결과: %d \n", result);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}