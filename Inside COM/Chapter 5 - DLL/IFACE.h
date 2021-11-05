#include <windows.h>
//
//Iface.h//
//

interface IX : IUnknown
{
	virtual void __stdcall fX() = 0;
};

interface IY : IUnknown
{
	virtual void __stdcall fY() = 0;
};

interface IZ : IUnknown
{
	virtual void __stdcall fZ() = 0;
};

//forward references for guids 

extern "C"
{
	extern const IID IID_IX;
	extern const IID IID_IY;
	extern const IID IID_IZ;
}






