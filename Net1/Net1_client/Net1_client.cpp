/*
	�ۼ��� : ��ٽ�
	�й� : 201232016
	�а� : ����Ʈ������а�
	���� : ����_Ŭ���̾�Ʈ
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
	int result = 0, opndCnt = 0, i = 0;

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

	memset(&opmsg, 0, sizeof(opmsg));

	fputs("�� ������ ��: ", stdout);
	scanf("%d", &opndCnt);
	opmsg[0] = (char)opndCnt;

	for (i = 0; i<opndCnt; i++) {
		printf("%d ��° �� ������ : ", i + 1);
		scanf("%d", (int*)&opmsg[i*OPSZ + 1]);
	}

	fgetc(stdin);
	for (i = 1; i < opndCnt; i++) {
		printf("%d ��° ������ : ", i);
		scanf("%c", &opmsg[opndCnt*OPSZ + i]);
		fgetc(stdin);
	}

	send(hSocket, opmsg, opndCnt*OPSZ + opndCnt, 0);

	recv(hSocket, (char*)&result, RLT_SIZE, 0);

	printf("���� ���: %d \n", result);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}