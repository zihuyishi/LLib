// DebugAndSendMail.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "SendMail.h"
#include <iostream>
void FunA();
int g_iVal = 0;
int _tmain(int argc, _TCHAR* argv[])
{
	DWORD dwTickStart = ::GetTickCount();
	//FunA();
	DWORD dwTicks = ::GetTickCount() - dwTickStart;
	std::cout << "ticks : " << dwTicks << std::endl;
	SEND_DEBUG_INFO();
	WAIT_DEBUG_INFO();
	dwTicks = ::GetTickCount() - dwTickStart;
	std::cout << "ticks : " << dwTicks << std::endl;
	dwTickStart = ::GetTickCount();
#pragma omp parallel for
	for (int i = 0; i < 1000; ++i) {
		CAT_ENTER_FUNC();
		CAT_STRING_INFO(i);
	}
	dwTicks = ::GetTickCount() - dwTickStart;
	std::cout << "ticks : " << dwTicks << std::endl;
	SEND_DEBUG_INFO();
	WAIT_DEBUG_INFO();
	dwTicks = ::GetTickCount() - dwTickStart;
	std::cout << "ticks : " << dwTicks << std::endl;
#pragma omp parallel for num_threads(4)
	for (int i = 0; i < 20; ++i) {
		std::cout << i << std::endl;
	}
	return 0;
}
DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
	int* iVal = (int*)lpParam;
	CAT_STRING_INFO(*iVal);
	return 0;
}
void FunA()
{
	HANDLE hThreads[4];
	DWORD dwResult;
	int iVals[] = { 1, 2, 3, 4 };
	for (int i = 0; i < 4; ++i)
	{
		hThreads[i] = ::CreateThread(
			NULL,
			0,
			ThreadFunc,
			&iVals[i],
			0,
			&dwResult);
	}
	dwResult = WaitForMultipleObjects(
		4,
		hThreads,
		TRUE,
		20000);
	for (int i = 0; i < 4; ++i) {
		if (hThreads[i]) {
			CloseHandle(hThreads[i]);
		}
	}
}