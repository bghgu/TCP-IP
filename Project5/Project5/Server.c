#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void ErrorHandling(char* message);

int main(int argc, char* argv[]) {

	WSADATA	wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	int strLen;
	char Connection_message[] = "Connection Complete...!!";
	char buff[100];

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	//문 소켓
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//atoi : ASCII를 Integer로
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	//listen(소켓, 연결 요청 대기 큐 갯수)
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	szClntAddr = sizeof(clntAddr);

	//클라이언트 연결 요청이 들어오면 헨들링을 리턴 한다.
	//큐에서 하나씩 연결 한다.
	//실제로 클라이언트와 통신할 때 쓰는 소켓
	//이 소켓을 통해 메시지를 주고 받는다.
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error");

	send(hClntSock, Connection_message, sizeof(Connection_message), 0);

	while (1)
	{
		//인풋 버퍼 초기화 해 주는 것이 좋다.
		memset(buff, 0, sizeof(char) * 100);
		strLen = recv(hClntSock, buff, sizeof(buff) - 1, 0);
		if (strLen == -1)
			ErrorHandling("read() error!");
		else
			if (strcmp(buff, "bye") == 0) break;
			printf("Message from client: %s \n", buff);
	}
	
	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();

	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

