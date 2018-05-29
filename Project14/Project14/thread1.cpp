#include <stdio.h>
#include <Windows.h> //WINAPI
#include <process.h> //_beginthreadex, _endthreadex

//������ ���� api ȣ�� �Ծ�
unsigned WINAPI ThreadFunc(void *arg);

int main(int argc, char *argv[]) {

	//�ڵ��� ����ü
	HANDLE hThread;
	unsigned threadID;
	int param = 5;

	//��ȯ������ �������� �ڵ鰪
	//ThreadFunc : ������ ���� �Լ�
	//(void*)&param : ������� �ѱ� �Ű�����
	//&threadID : ������ id
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