#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>

//Example
extern "C++" __declspec(dllexport) void LetterList()
{
	std::cout << "this function was called from LetterList()" << std::endl;
}

extern "C++" __declspec(dllexport) int PutInt(int param)
{
	return param;
}

//Task1
extern "C++" __declspec(dllexport) void PrintToFile(char* file_name, char* str)
{
	FILE* file = fopen(file_name, "w");
	fprintf(file, str);
	fclose(file);
}

//Task2
extern "C++" __declspec(dllexport) void StartProcess(wchar_t* path_to_process)
{
	STARTUPINFO cif;
	ZeroMemory(&cif, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	if (CreateProcess(path_to_process, NULL,
		NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi) == TRUE)
	{
		_getch();				
		TerminateProcess(pi.hProcess, NO_ERROR);	
	}
}

//Task3
extern "C++" __declspec(dllexport) bool CheckFile(char* directory, char* file_name)
{
	char* path_to_file = new char[200];
	sprintf(path_to_file, "%s\\%s", directory, file_name);
	wchar_t* path = new wchar_t[200];
	mbstowcs(path, path_to_file, 200);
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(path, &data);
	return hFind != INVALID_HANDLE_VALUE;
}