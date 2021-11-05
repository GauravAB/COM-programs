#ifndef _IFACE_H_
#define _IFACE_H_

interface IX : IUnknown
{
	virtual void __stdcall Fx() = 0;
};

interface IY : IUnknown
{
	virtual void __stdcall Fy() = 0;
};

interface IZ : IUnknown
{
	virtual void __stdcall Fz() = 0;
};


extern "C" IID IID_IX;
extern "C" IID IID_IY;
extern "C" IID IID_IZ;
extern "C" CLSID CLSID_Component1;
extern "C" CLSID CLSID_Component2;


#endif