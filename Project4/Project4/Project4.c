#include <stdio.h>
#include <WinSock2.h>
#include <string.h>

void ErrorHandling(char* message);

int main(int argc, char *argv[]) {


	WSADATA wsaData;
	//윈속의 초기화
	//윈속의 기능을 이용하기 전에는 반드시 윈속의 초기화를 해야 된다.
	//첫번째 인수 : 버전
	//MAKEWORD : 2개의 인수를 WORD로 pack 하는 매크로(2.2 버전 사용)
	//두번째 인수 : WSAStartup()이 초기화된 상태를 저장하는 변수를 넘겨준다.
	//따라서 WSAStartup()은 윈속을 초기화하고 초기화한 상태를 WSADATA 구조체에 저장한다.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	//inet_addr 함수 호출 예시
	{
		char *addr = "127.212.124.78";
		unsigned long conv_addr = inet_addr(addr);
		if (conv_addr == INADDR_NONE)
			printf("Error occured!\n");
		else
			printf("Network ordered integer addr : %#lx\n", conv_addr);
	}

	//inet_ntoa 함수 호출 예시
	{
		struct sockaddr_in addr;
		char *strPtr;
		char strArr[20];

		addr.sin_addr.s_addr = htonl(0x1020304);
		strPtr = inet_ntoa(addr.sin_addr);
		strcpy(strArr, strPtr);
		printf("Dotted-Decimal notation3 %s\n", strArr);
	}
	
	//윈속의 기능 종료
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
