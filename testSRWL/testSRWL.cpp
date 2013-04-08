// testSRWL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>

typedef struct _tagTESTITEM
{
	void*	pItem;
	SRWLOCK	srwl;
	//CRITICAL_SECTION cs;
}
TESTITEM, *PTESTITEM;

PTESTITEM g_pti = NULL;
SRWLOCK	g_srwl;

DWORD  WINAPI func(void* p)
{
	int n = (int)p;	
	//EnterCriticalSection(&(g_pti[n].cs));
	AcquireSRWLockExclusive(&(g_pti[n].srwl));
	//AcquireSRWLockExclusive(&g_srwl);
	printf("func(%d) begin\n", n);
	Sleep(n*100);
	printf("func(%d) end\n", n);
	//LeaveCriticalSection(&(g_pti[n].cs));
	ReleaseSRWLockExclusive(&(g_pti[n].srwl));
	//ReleaseSRWLockExclusive(&g_srwl);
	return 0;
}

DWORD g_dw1 = 0;
DWORD g_dw2 = 0;
DWORD g_dw3 = 0;
DWORD g_dw4 = 0;
DWORD g_dw5 = 0;
DWORD g_dw6 = 0;
HANDLE g_hEvent1 = NULL;
HANDLE g_hEvent2 = NULL;
int g_nLoopCnt = 300;
int g_nThreadCnt = 500;

DWORD WINAPI func1(void* p)
{
	while(WaitForSingleObject(g_hEvent1, INFINITE) == WAIT_OBJECT_0)
	{
		for(int i=0; i<g_nLoopCnt; i++)
		{
			
			InterlockedIncrement((long*)&g_dw1);			
			InterlockedExchange((long*)&g_dw2, g_dw1);
			InterlockedExchangeAdd((long*)&g_dw3, 2425);

			/*AcquireSRWLockExclusive(&g_srwl);
			g_dw1++;
			ReleaseSRWLockExclusive(&g_srwl);
			AcquireSRWLockExclusive(&g_srwl);
			g_dw2++;
			ReleaseSRWLockExclusive(&g_srwl);
			AcquireSRWLockExclusive(&g_srwl);
			g_dw3++;
			ReleaseSRWLockExclusive(&g_srwl);*/

			g_dw1++;
			g_dw2++;
			g_dw3++;
			
		}
		InterlockedIncrement((long*)&g_dw4);
		WaitForSingleObject(g_hEvent2, INFINITE);		
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	InitializeSRWLock(&(g_srwl));
	g_pti = new TESTITEM[200000];
	
	DWORD dwLast = GetTickCount();
	for(int i=0; i< 200000; i++)
	{
		g_pti[i].pItem = NULL;
		InitializeSRWLock(&(g_pti[i].srwl));
		//InitializeCriticalSection(&(g_pti[i].cs));
	}
	printf("InitializeSRWLock[200000] = %d\n", GetTickCount()-dwLast);

	for(int i=0; i<1000; i++)
	{
		DWORD dwID;
		HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)func, (void*)i, NULL, &dwID);
		if(hThread != NULL)	CloseHandle(hThread);
	}
	int nstep = 0;
	while(1)
	{
		printf("main() %d\n", ++nstep);
		Sleep(1000);
	}
	return 0;

	/*InitializeSRWLock(&g_srwl);
	g_hEvent1 = CreateEvent(NULL, TRUE, FALSE, NULL);
	g_hEvent2 = CreateEvent(NULL, TRUE, FALSE, NULL);
	for(int i=0; i<g_nThreadCnt; i++)
	{
		DWORD dwID;
		HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)func1, (void*)i, NULL, &dwID);
		if(hThread != NULL)	CloseHandle(hThread);
	}

AAAA:	
	InterlockedExchange((long*)&g_dw1, 0);
	InterlockedExchange((long*)&g_dw2, 0);
	InterlockedExchange((long*)&g_dw3, 0);	
	InterlockedExchange((long*)&g_dw4, 0);	
	InterlockedExchange((long*)&g_dw5, 0);	
	InterlockedExchange((long*)&g_dw6, 0);	
	DWORD dwStart = GetTickCount();
	ResetEvent(g_hEvent2);
	SetEvent(g_hEvent1);
	while(1)
	{
		if(g_dw4 == g_nThreadCnt)
		{
			printf("Total Span = %d\n", GetTickCount()-dwStart);
			ResetEvent(g_hEvent1);
			SetEvent(g_hEvent2);
			break;
		}
		Sleep(10);
	}
	printf("Press any key......!\n");
	getchar();	
	goto AAAA;
	return 0;*/
}



