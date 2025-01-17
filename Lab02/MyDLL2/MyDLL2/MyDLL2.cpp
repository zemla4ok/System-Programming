// MyDLL2.cpp: определяет процедуры инициализации для библиотеки DLL.
//

#include "stdafx.h"
#include "MyDLL2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

__declspec(dllexport) int Sum(int a, int b);
__declspec(dllexport) int Mul(int a, int b);
__declspec(dllexport) int Div(int a, int b);

int Sum(int a, int b)
{
	return a + b;
}

int Mul(int a, int b)
{
	return a * b;
}

int Div(int a, int b)
{
	return a / b;
}

//
//TODO: если эта библиотека DLL динамически связана с библиотеками DLL MFC,
//		все функции, экспортированные из данной DLL-библиотеки, которые выполняют вызовы к
//		MFC, должны содержать макрос AFX_MANAGE_STATE в
//		самое начало функции.
//
//		Например:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// тело нормальной функции
//		}
//
//		Важно, чтобы данный макрос был представлен в каждой
//		функции до вызова MFC.  Это означает, что
//		должен стоять в качестве первого оператора в
//		функции и предшествовать даже любым объявлениям переменных объекта,
//		поскольку их конструкторы могут выполнять вызовы к MFC
//		DLL.
//
//		В Технических указаниях MFC 33 и 58 содержатся более
//		подробные сведения.
//

// CMyDLL2App

BEGIN_MESSAGE_MAP(CMyDLL2App, CWinApp)
END_MESSAGE_MAP()


// Создание CMyDLL2App

CMyDLL2App::CMyDLL2App()
{
	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
}


// Единственный объект CMyDLL2App

CMyDLL2App theApp;


// Инициализация CMyDLL2App

BOOL CMyDLL2App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
