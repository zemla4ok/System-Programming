#include "stdafx.h"
#include "string.h"
#include <iostream>
#include <ole2.h>

#define TASK_1
//#define TASK_2


#ifdef TASK_1
bool CheckFile(wchar_t* filename)
{
	WIN32_FIND_DATA wfd;
	HANDLE h;
	h = FindFirstFile(filename, &wfd);
	return h != INVALID_HANDLE_VALUE;
}


HRESULT AutoWrap(int autoType, VARIANT *pvResult, IDispatch *pDisp, LPOLESTR ptName, int cArgs...)
{
	// Begin variable-argument list...
	va_list marker;
	va_start(marker, cArgs); //формируем список аргументов, передаваемых в cArgs

	if (!pDisp) {
		MessageBox(NULL, TEXT("NULL IDispatch passed to AutoWrap()"),
			TEXT("Error"), 0x10010);
		_exit(0);
	}
	// pDisp – это объект, на котoром выполняется метод, напр. document
	// Variables used...
	DISPPARAMS dp = { NULL, NULL, 0, 0 }; //структура определяет параметры,
										  // передаваемые в метод
	DISPID dispidNamed = DISPID_PROPERTYPUT;
	DISPID dispID;
	HRESULT hr;
	char buf[200];
	char szName[200];

	// Convert down to ANSI
	WideCharToMultiByte(CP_ACP, 0, ptName, -1, szName, 256, NULL, NULL);

	// Get DISPID for name passed...
	hr = pDisp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT,
		&dispID);
	if (FAILED(hr)) {
		sprintf(buf,
			"IDispatch::GetIDsOfNames(\"%s\") failed w/err0x%08lx",
			szName, hr);
		MessageBox(NULL, TEXT("Click"), TEXT("AutoWrap()"), 0x10010);
		_exit(0);
		return hr;
	}

	// Allocate memory for arguments...
	VARIANT *pArgs = new VARIANT[cArgs + 1];

	// Extract arguments...
	for (int i = 0; i<cArgs; i++) {
		pArgs[i] = va_arg(marker, VARIANT);
	}

	// Build DISPPARAMS
	dp.cArgs = cArgs;
	dp.rgvarg = pArgs;

	// Handle special-case for property-puts!
	if (autoType & DISPATCH_PROPERTYPUT) {
		dp.cNamedArgs = 1;
		dp.rgdispidNamedArgs = &dispidNamed;
	}

	// Make the call!
	hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, autoType, &dp, pvResult, NULL, NULL);
	if (FAILED(hr)) {
		/*sprintf(buf,
			"IDispatch::Invoke(\"%s\"=%08lx) failed w/err 0x%08lx",
			szName, dispID, hr);
			*/
		printf("IDispatch::Invoke(\"%s\"=%08lx) failed w/err 0x%08lx",szName, dispID, hr);
		MessageBox(NULL, TEXT("Click Me"), TEXT("AutoWrap()"), 0x10010);
		_exit(0);
		return hr;
	}
	// End variable-argument section...
	va_end(marker);
	delete[] pArgs;
	return hr;

}

int main(int argc, char* argv[])
{
	SetConsoleCP(1251); // настраиваем кодировку консоли (ввод)
	SetConsoleOutputCP(1251); // (вывод)
	setlocale(LC_ALL, "");
	// Initialize COM for this thread...
	CoInitialize(NULL);

	// Get CLSID for Word.Application...
	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(L"Word.Application", &clsid);
	if (FAILED(hr)) {
		::MessageBox(NULL, TEXT("CLSIDFromProgID() failed"), TEXT("Error"),
			0x10010);
		return -1;
	}
	// Start Word and get IDispatch...
	IDispatch *pWordApp;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER,
		IID_IDispatch, (void **)&pWordApp);
	if (FAILED(hr)) {
		::MessageBox(NULL, TEXT("Word not registered properly"),
			TEXT("Error"), 0x10010);
		return -2;
	}

	// Make Word visible
	{
		VARIANT x;
		x.vt = VT_I4;
		x.lVal = 1;
		AutoWrap(DISPATCH_PROPERTYPUT, NULL, pWordApp, (wchar_t*)L"Visible", 1, x);
	}
	// Get Documents collection
	IDispatch *pDocs;
	{
		VARIANT result;
		VariantInit(&result);
		AutoWrap(DISPATCH_PROPERTYGET, &result, pWordApp, (wchar_t*)L"Documents", 0);
		pDocs = result.pdispVal;
	}

	// Call Documents.Open() to open 
	IDispatch *pDoc;
	{
		VARIANT result;
		VariantInit(&result);
		puts("Enter filename:");
		//char* filename = new char[200];
		wchar_t* filename_utf = new wchar_t[200];
		
	    std::wcin >> filename_utf;
		bool rc = CheckFile(filename_utf);
		if (!rc)
		{
			puts("There is no file");
			return 0;
		}
		VARIANT x;
		x.vt = VT_BSTR;
		x.bstrVal = ::SysAllocString((const wchar_t*)filename_utf);
		AutoWrap(DISPATCH_METHOD, &result, pDocs, (wchar_t*)L"Open", 1, x);
		pDoc = result.pdispVal;
		SysFreeString(x.bstrVal);
	}

	::MessageBox(NULL,
		TEXT("Click Me"),
		TEXT("word doc"), 0x10000);
	pDoc->Release();
	pDocs->Release();
	pWordApp->Release();
	// Uninitialize COM for this thread...
	CoUninitialize();
	return 0;
}
#elif defined TASK_2
typedef void(*FUNCTION)(wchar_t*);

int main(int argc, char* argv[])
{
	HMODULE dll = LoadLibraryW((wchar_t*)L"OS2_4_DLL.dll");
	FUNCTION word = (FUNCTION)GetProcAddress(dll, "OpenWordFile");
	word((wchar_t*)L"e:\\Лаб_13_COM_CLSID.doc");
	system("pause");
	return 0;
}
#endif