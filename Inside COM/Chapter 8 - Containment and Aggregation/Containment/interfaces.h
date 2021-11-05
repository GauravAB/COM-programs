#include <Windows.h>


interface IX : IUnknown
{
	virtual void __stdcall Fx() = 0;
};

interface IY : IUnknown
{
	virtual void __stdcall Fy() = 0;
};

//these constants are defined in guid.cpp

extern "C" const IID IID_IX;
extern "C" const IID IID_IY;
extern "C" const CLSID CLSID_Component1;
extern "C" const CLSID CLSID_Component2;

