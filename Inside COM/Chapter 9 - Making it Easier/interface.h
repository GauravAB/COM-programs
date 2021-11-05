#ifndef __IFACE_H__
#define __IFACE_H__

#include <Windows.h>

interface IX : public IUnknown
{
	virtual void __stdcall Fx() = 0;
};

interface IY : public IUnknown
{
	virtual void __stdcall Fy() = 0;
};

interface IZ : public IUnknown
{
	virtual void __stdcall Fz() = 0;
};


extern "C" const IID IID_IX;
extern "C" const IID IID_IY;
extern "C" const IID IID_IZ;

extern "C" const CLSID CLSID_Component1;
extern "C" const CLSID CLSID_Component2;
extern "C" const CLSID CLSID_Component3;


#endif

