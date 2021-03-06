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
	FILE* fp;
	char file_name[BUF_SIZE];
	char msgyes[] = "File Exist";
	char msgno[] = "No File";

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

	//파일 이름 수신
	recv(hClntSock, (char*)(&str_len), 4, 0);
	recv(hClntSock, file_name, str_len, 0);
	puts(file_name);
	
	fopen_s(&fp, file_name, "r");
	if (fp != NULL) {
		str_len = strlen(msgyes) + 1;
		send(hClntSock, (char*)(&str_len), 4, 0);
		send(hClntSock, msgyes, str_len, 0);
	}
	else {
		str_len = strlen(msgno) + 1;
		send(hClntSock, (char*)(&str_len), 4, 0);
		send(hClntSock, msgno, str_len, 0);
	}
	
	//fclose(fp);

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