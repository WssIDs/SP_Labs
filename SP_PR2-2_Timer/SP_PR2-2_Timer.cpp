// SP_PR2-2_Timer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


DWORD WINAPI ThreadFunc(PVOID pvParam);
int cAbrasionCount = 0;

void main(void)
{
	const int nTimerUnitsPerSecond = 10000000;
	LARGE_INTEGER li;
	SYSTEMTIME st;

	HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, NULL);

	printf("\nWaitableTimer Volod'ko");
	printf("\nwait 2 minutes, after 11 seconds shown message\n\n");
	GetLocalTime(&st);
	printf("Start time: \t\t%.2d:%.2d:%.2d\n\n", st.wHour, st.wMinute, st.wSecond);

	li.QuadPart = -(120 * nTimerUnitsPerSecond);
	if (SetWaitableTimer(hTimer, &li, 11 * 1000, NULL, NULL, FALSE)) {
		while (TRUE) {
			WaitForSingleObject(hTimer, INFINITE);
			DWORD ThreadId;
			HANDLE hThread = CreateThread(NULL, NULL, ThreadFunc, 0, 0, &ThreadId);
		}
	}
}

DWORD WINAPI ThreadFunc(PVOID pvParam)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	cAbrasionCount++;
	printf("Signal # <%d>: \t%.2d:%.2d:%.2d\n", cAbrasionCount, st.wHour, st.wMinute, st.wSecond);
	return 0;
}
