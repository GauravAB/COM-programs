#include <Windows.h>


interface IX : IUnknown
{
	virtual void pascal Fx() = 0;
};

interface IY : IUnknown
{
	virtual void pascal Fy() = 0;
};

interface IZ : IUnknown
{
	virtual void pascal Fz() = 0;
};

//Declaration of GUIDs for interfaces and component
//These constants are defined in GUID.cpp

extern "C" const IID IID_IX;
extern "C" const IID IID_IY;
extern "C" const IID IID_IZ;
extern "C" const CLSID CLSID_Component1;
