// EdrLib2.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
extern "C++" __declspec(dllexport) BOOL EdrCenterText(HDC hdc, PRECT prc, LPCWSTR pStr)
{

	int iLength;
	SIZE size;
	iLength = wcslen(pStr);
	GetTextExtentPoint32(hdc, pStr, iLength, &size);
	return TextOut(hdc, (prc->right - prc->left - size.cx) / 2,
		(prc->bottom - prc->top - size.cy) / 2,
		pStr, iLength);
}
