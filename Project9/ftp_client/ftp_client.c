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
	char file_name[BUF_SIZE];

	int str_len, fsize = 0;

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

	//버퍼 0으로 초기화
	memset(read_buf, 0, sizeof(char) * 1024);
	memset(file_name, 0, sizeof(char) * 1024);

	fputs("Input File Name : ", stdout);
	//fgets(file_name, 128, stdin);
	//fgets는 개행문자 들어가서 처리해줘야 한다.
	//file_name[strlen(file_name) - 1] = '\0';
	scanf("%s", file_name);

	str_len = strlen(file_name) + 1;
	send(hSocket, (char*)(&str_len), 4, 0);
	send(hSocket, file_name, str_len, 0);

	recv(hSocket, (char*)(&str_len), 4, 0);
	recv(hSocket, read_buf, str_len, 0);
	puts(read_buf);

	memset(read_buf, 0, sizeof(char) * 1024);
	FILE* fp = fopen("receive.txt", "wt");

	while ((fsize = recv(hSocket, read_buf, BUF_SIZE, 0)) != 0)
		fwrite(read_buf, 1, BUF_SIZE, fp);

	fclose(fp);
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}