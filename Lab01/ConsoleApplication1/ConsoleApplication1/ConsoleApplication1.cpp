#include "stdafx.h"
#include "dll.h"
#include <iostream>
#include <conio.h>

int main()
{
	int x = PutInt(5);
	LetterList();

	std::cout << x << std::endl;

	PrintToFile((char*)"file.txt", (char*)"hello world");

	StartProcess((wchar_t*)L"C:\\Program Files (x86)\\Solo9RusEngNum\\Solo.exe");

	std::cout << CheckFile((char*)"E:\\3k2s\\SP\\OS\\Lab01\\ConsoleApplication1\\ConsoleApplication1", (char*)"file.txt") << std::endl;

	return 0;
}
