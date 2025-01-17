#include "stdafx.h"
#include <windows.h>
#include <iostream>

typedef int(__stdcall *f_funci)();
typedef int(*FUNCTION)(int, int);


int main()
{
	HINSTANCE hProcID = LoadLibrary(TEXT("E:\\3k2s\\SP\\OS\\Lab02\\MyDLLMFC\\Debug\\MyDLLMFC.dll"));

	if (!hProcID) {
		std::cout << "could not load the dynamic library\n" << GetLastError() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Dll found!!!!" << std::endl;

	f_funci funci = (f_funci)GetProcAddress(hProcID, "SendMessageNumber");

	if (!funci) {
		std::cout << "could not locate the function\n" << GetLastError() << std::endl;
		getchar();
		FreeLibrary(hProcID);
		return EXIT_FAILURE;
	}

	std::cout << "SendMessageNumber found " << std::endl;

	int x = (funci)();
	std::cout << "SendMessage returned " << x << std::endl;

	getchar();
	FreeLibrary(hProcID);

	//****************************************************************************************
	//****************************************************************************************
	//****************************************************************************************
	//****************************************************************************************

	HINSTANCE MyDll2 = LoadLibrary(TEXT("E:\\3k2s\\SP\\OS\\Lab02\\MyDLL2\\Debug\\MyDLL2.dll"));

	if (!MyDll2) {
		std::cout << "could not load the dynamic library\n" << GetLastError() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Dll found!!!!" << std::endl;
	
	FUNCTION sum = (FUNCTION)GetProcAddress(MyDll2, "Sum");

	if (!sum) {
		std::cout << "could not locate the function\n" << GetLastError() << std::endl;
		getchar();
		FreeLibrary(MyDll2);
		return EXIT_FAILURE;
	}

	int res1 = (sum)(4, 2);
	std::cout << "4+2=" << res1 << std::endl;

	//****************************************************************************************

	FUNCTION mul = (FUNCTION)GetProcAddress(MyDll2, "Mul");

	if (!mul) {
		std::cout << "could not locate the function\n" << GetLastError() << std::endl;
		getchar();
		FreeLibrary(MyDll2);
		return EXIT_FAILURE;
	}

	int res2 = (mul)(4, 2);
	std::cout << "4*2=" << res2 << std::endl;	

	//****************************************************************************************

	FUNCTION div = (FUNCTION)GetProcAddress(MyDll2, "Div");

	if (!div) {
		std::cout << "could not locate the function\n" << GetLastError() << std::endl;
		getchar();
		FreeLibrary(MyDll2);
		return EXIT_FAILURE;
	}

	int res3 = (div)(4, 2);
	std::cout << "4/2=" << res3 << std::endl;

	getchar();
	FreeLibrary(MyDll2);

	return EXIT_SUCCESS;
}