#include <stdio.h>
#include <Windows.h> //WINAPI
#include <process.h> //_beginthreadex, _endthreadex

//윈도우 제공 api 호출 규약
unsigned WINAPI ThreadFunc(void *arg);

int main(int argc, char *argv[]) {

	//핸들형 구조체
	HANDLE hThread;
	unsigned threadID;
	int param = 5;

	//반환값으로 스레드의 핸들값
	//ThreadFunc : 스레드 메인 함수
	//(void*)&param : 스레드로 넘길 매개변수
	//&threadID : 스레드 id
	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)&param, 0, &threadID);
	
	if (hThread == 0) {
		puts("_beginthreadex() error");
		return -1;
	}

	Sleep(3000);
	
	puts("end of main");
	
	return 0;
}

unsigned WINAPI ThreadFunc(void *arg) {
	int i;
	int cnt = *((int*)arg);

	for (i = 0; i < cnt; i++) {
		Sleep(1000);
		puts("running thread");
	}

	return 0;
}