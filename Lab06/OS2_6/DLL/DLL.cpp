// DLL.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include <windows.h>                // Program Demonstrates Late Bound OLE COM Access To MS Excel Spreadsheet Using C++.
#include <cstdio>   
#include <OAIdl.h>
const CLSID CLSID_XLApplication = { 0x00024500,0x0000,0x0000,{ 0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46 } }; // CLSID of Excel
const IID   IID_Application = { 0x000208D5,0x0000,0x0000,{ 0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46 } }; // IID of _Application


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
	
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	DISPID dispidNamed = DISPID_PROPERTYPUT;
	DISPID dispID;
	HRESULT hr;
	char buf[200];
	char szName[200];
	WideCharToMultiByte(CP_ACP, 0, ptName, -1, szName, 256, NULL, NULL);
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
	for (int i = 0; i < cArgs; i++) {
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
	hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, autoType,
		&dp, pvResult, NULL, NULL);
	if (FAILED(hr)) {
		sprintf(buf,
			"IDispatch::Invoke(\"%s\"=%08lx) failed w/err 0x%08lx",
			szName, dispID, hr);
		MessageBox(NULL, TEXT("Click Me"), TEXT("AutoWrap()"), 0x10010);
		_exit(0);
		return hr;
	}
	// End variable-argument section...
	va_end(marker);

	delete[] pArgs;

	return hr;

}

extern "C" _declspec(dllexport) void WorkWithExcel()
{
	DISPPARAMS NoArgs = { NULL,NULL,0,0 }; // This variable is used in easiest Invoke() call when the method has no parameters.  When
	IDispatch* pXLApp = NULL;            // using the IDispatch interface in conjunction with Invoke() method parameters must be loaded
	DISPPARAMS DispParams;             // into a DISPPARAMS struct.  The actual parameters are loaded into VARIANTs, and one of the
	VARIANT CallArgs[1];               // members of the DISPPARAMS struct is a pointer to the array of VARIANT.  The other members
	VARIANT vResult;                   // of the DISPARAMS struct tell Invoke() how many parameters are being passed, as well as other
	DISPID dispid;                     // specifics such as the type of the call (propput, propget, etc.).
	HRESULT hr;

	CoInitialize(NULL);
	hr = CoCreateInstance(CLSID_XLApplication, NULL, CLSCTX_LOCAL_SERVER, IID_Application, (void**)&pXLApp);
	if (SUCCEEDED(hr))
	{
		OLECHAR* szVisible = (OLECHAR*)L"Visible";
		hr = pXLApp->GetIDsOfNames(IID_NULL, &szVisible, 1, GetUserDefaultLCID(), &dispid);
		if (SUCCEEDED(hr))
		{
			VariantInit(&CallArgs[0]);
			CallArgs[0].vt = VT_BOOL;
			CallArgs[0].boolVal = TRUE;
			DISPID dispidNamed = DISPID_PROPERTYPUT;
			DispParams.rgvarg = CallArgs;
			DispParams.rgdispidNamedArgs = &dispidNamed;
			DispParams.cArgs = 1;
			DispParams.cNamedArgs = 1;
			VariantInit(&vResult);       //  Call or Invoke _Application::Visible(true);
			hr = pXLApp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &DispParams, &vResult, NULL, NULL);
			OLECHAR* szWorkbooks = (OLECHAR*)L"Workbooks";
			hr = pXLApp->GetIDsOfNames(IID_NULL, &szWorkbooks, 1, GetUserDefaultLCID(), &dispid);
			if (SUCCEEDED(hr))
			{
				IDispatch* pXLBooks = NULL;    //  Get Workbooks Collection
				VariantInit(&vResult);       //  Invoke _Application::Workbooks(&pXLBooks) << returns IDispatch** of Workbooks Collection
				hr = pXLApp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &NoArgs, &vResult, NULL, NULL);
				if (SUCCEEDED(hr))
				{
					pXLBooks = vResult.pdispVal;
					IDispatch* pXLBook = NULL;  //  Try to add Workbook
					OLECHAR* szAdd = (OLECHAR*)L"Add";
					hr = pXLBooks->GetIDsOfNames(IID_NULL, &szAdd, 1, GetUserDefaultLCID(), &dispid);
					if (SUCCEEDED(hr))
					{
						VariantInit(&vResult);    //  Invoke Workbooks::Add(&Workbook)  << returns IDispatch** of Workbook Object
						hr = pXLBooks->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD | DISPATCH_PROPERTYGET, &NoArgs, &vResult, NULL, NULL);
						if (SUCCEEDED(hr))
						{
							pXLBook = vResult.pdispVal;
							OLECHAR* szActiveSheet = (OLECHAR*)L"ActiveSheet";
							hr = pXLApp->GetIDsOfNames(IID_NULL, &szActiveSheet, 1, GetUserDefaultLCID(), &dispid);
							if (SUCCEEDED(hr))
							{
								IDispatch* pXLSheet = NULL;  // Try To Get ActiveSheet
								VariantInit(&vResult);     // Invoke _Application::ActiveSheet(&pXLSheet);  << ret IDispatch** to Worksheet (Worksheet)
								hr = pXLApp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &NoArgs, &vResult, NULL, NULL);
								if (SUCCEEDED(hr))
								{
									pXLSheet = vResult.pdispVal;
									OLECHAR* szRange = (OLECHAR*)L"Range";
									hr = pXLSheet->GetIDsOfNames(IID_NULL, &szRange, 1, GetUserDefaultLCID(), &dispid);
									if (SUCCEEDED(hr))
									{
										IDispatch* pXLRange = NULL;
										VariantInit(&vResult);
										CallArgs[0].vt = VT_BSTR,
											CallArgs[0].bstrVal = SysAllocString(L"A5");
										DispParams.rgvarg = CallArgs;
										DispParams.rgdispidNamedArgs = 0;
										DispParams.cArgs = 1;  // Try to get Range
										DispParams.cNamedArgs = 0;  // Invoke _Worksheet::Range("A1")  << returns IDispatch** to dispinterface Range
										hr = pXLSheet->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &DispParams, &vResult, NULL, NULL);
										if (SUCCEEDED(hr))
										{
											IDispatch* pXLRange = vResult.pdispVal;
											VARIANT result;
											VariantInit(&result);
											hr = AutoWrap(DISPATCH_PROPERTYGET, &result, pXLRange, (wchar_t*)L"Font", 0);
											if (SUCCEEDED(hr))
											{
												IDispatch* pXLFont = result.pdispVal;
												VARIANT x;
												x.vt = VT_BOOL;
												x.boolVal = TRUE;
												hr = AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXLFont, (wchar_t*)L"Bold", 1, x);
												VARIANT y;
												y.vt = VT_INT;
												y.intVal = 20;
												hr = AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXLFont, (wchar_t*)L"Size", 1, y);
												VARIANT z;
												z.vt = VT_I4;
												z.lVal = RGB(255, 255, 0);
												hr = AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXLFont, (wchar_t*)L"Color", 1, z);
											}

											//OLECHAR* szValue = (OLECHAR*)L"Value";
											VARIANT x;
											x.vt = VT_BSTR;
											x.bstrVal = SysAllocString(L"zemla4ok");
											hr = AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXLRange, (wchar_t*)L"Value", 1, x);
											pXLRange->Release();
										}
									}
									pXLSheet->Release();
								}
							}
							pXLBook->Release();
						}
					}
					pXLBooks->Release();
				}
			}
			getchar();
		}
		VariantInit(&vResult);  // Try to do _Application::Close()
		hr = pXLApp->Invoke(0x0000012e, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &NoArgs, &vResult, NULL, NULL);
		pXLApp->Release();
	}
	CoUninitialize();

	return;
}