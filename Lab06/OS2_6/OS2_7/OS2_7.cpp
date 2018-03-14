#include "stdafx.h"
#include <windows.h>                // Program Demonstrates Late Bound OLE COM Access To MS Excel Spreadsheet Using C++.
                  // Interface Using GetIDsOfNames() And Invoke() Used Throughout.
typedef void(*FUNCTION)();
int main()
{
	HMODULE dll = LoadLibrary(L"DLL.dll");
	FUNCTION excel = (FUNCTION)GetProcAddress(dll, "WorkWithExcel");
	excel();
	system("pause");
	return 0;
}
