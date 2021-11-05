#include <Windows.h>


interface IX : IUnknown
{
	//function to be implemented by the concrete class
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

extern "C" const IID IID_IX;
extern "C" const IID IID_IY;
extern "C" const IID IID_IZ;
extern "C" const CLSID CLSID_Component1;

