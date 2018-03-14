#pragma once
#include <objbase.h>

interface IX :IUnknown
{
	virtual void __stdcall Fx() = 0;
	virtual int _stdcall Sqr(int a) = 0;
};

interface IY :IUnknown
{
	virtual void __stdcall Fy() = 0;
};