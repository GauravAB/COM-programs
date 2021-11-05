#ifndef _IFACE_H
#define _IFACE_H




interface IX : IUnknown
{
	virtual void __stdcall Fx() = 0;
};

interface IY : IUnknown
{
	virtual void __stdcall Fy() = 0;
}



interface IZ : IUnknown
{
	virtual void __stdcall Fz() = 0;
}



extern "C" const IID IID_IY;
extern "C" const IID IID_IX;
extern "C" const IID IID_IZ;
extern "C" const CLSID CLSID_Component2;
extern "C" const CLSID CLSID_Component1;

#endif