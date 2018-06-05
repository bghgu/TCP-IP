#include <stdio.h>
#include <windows.h>
#include <process.h>

#define STR_LEN 100

unsigned WINAPI NumberOfA(void *arg);
unsigned WINAPI NumberOfOthers(void *arg);

static char str[STR_LEN];
static HANDLE hEvent;
//semaphore
static HANDLE sema;

int main(int arg, char *argv[]) {
	HANDLE hThread1, hThread2;

	//true = manual reset
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//semaphore
	sema = CreateSemaphore(NULL, 0, 1, NULL);

	hThread1 = (HANDLE)_beginthreadex(NULL, 0, NumberOfA, NULL, 0, NULL);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, NumberOfOthers, NULL, 0, NULL);

	fputs("Input string: ", stdout);
	fgets(str, STR_LEN, stdin);
	SetEvent(hEvent);

	ReleaseSemaphore(sema, 1, NULL);

	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);

	ResetEvent(hEvent);

	CloseHandle(hEvent);
	CloseHandle(sema);

	return 0;
}

/*
동시에 읽기만 하기 때문에 문제가 없다.
*/

unsigned WINAPI NumberOfA(void *arg) {
	int i, cnt = 0;
	WaitForSingleObject(hEvent, INFINITE);
	
	//semaphore
	WaitForSingleObject(sema, INFINITE);

	for (i = 0; str[i] != 0; i++) {
		if (str[i] == 'A') cnt++;
	}
	printf("Num of A: %d\n", cnt);
	//semaphore
	ReleaseSemaphore(sema, 1, NULL);
	return 0;
}

unsigned WINAPI NumberOfOthers(void *arg) {
	int i, cnt = 0;
	WaitForSingleObject(hEvent, INFINITE);

	//semaphore
	WaitForSingleObject(sema, INFINITE);

	for (i = 0; str[i] != 0; i++) {
		if (str[i] != 'A') cnt++;
	}
	printf("Num of othrers: %d\n", cnt - 1);
	//semaphore
	ReleaseSemaphore(sema, 1, NULL);
	return 0;
}