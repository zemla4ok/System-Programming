// MyDLL2.h: основной файл заголовка для библиотеки DLL MyDLL2
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMyDLL2App
// Реализацию этого класса см. в файле MyDLL2.cpp
//

class CMyDLL2App : public CWinApp
{
public:
	CMyDLL2App();

// Переопределение
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
