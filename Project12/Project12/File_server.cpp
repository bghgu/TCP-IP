#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	FILE *fp;
	char buf[BUF_SIZE];
	int readCnt;

	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSz;

	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	fp = fopen("readme.txt", "rb");
	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	listen(hServSock, 5);

	clntAdrSz = sizeof(clntAdr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);
	
	/*
	파일 크기를 버퍼 만큼 자르면서 보낸다.
	버퍼 크기 보다 (남은)파일의 크기가 작다면, 그만큼만 보낸다.
	파일 크기를 모르기 때문에 그렇게 한다.
	*/
	while (1) {
		readCnt = fread((void*)buf, 1, BUF_SIZE, fp);
		if (readCnt < BUF_SIZE) {
			send(hClntSock, (char*)&buf, readCnt, 0);
			break;
		}
		send(hClntSock, (char*)&buf, BUF_SIZE, 0);
	}

	shutdown(hClntSock, SD_SEND);
	recv(hClntSock, (char*)buf, BUF_SIZE, 0);
	printf("Message from clinet : %s \n", buf);

	fclose(fp);
	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
