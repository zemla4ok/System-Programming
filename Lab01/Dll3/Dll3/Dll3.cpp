// Dll3.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include <ctime>
#include <stdio.h>

extern "C" __declspec(dllexport) int __stdcall AddNumbers(int x, int y)
{
	return x + y;
}

extern "C" __declspec(dllexport) void __stdcall GetMyCurrentTime()
{
	time_t t = time(NULL);
	puts(asctime(localtime(&t)));
}