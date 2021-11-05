#include "Component3.h"
#include "interface.h"
#include "UTIL.h"


static void trace(const char* msg)
{
	Util::Trace("Component 1 ", msg, S_OK);
}

static void trace(const char* msg, HRESULT hr)
{
	Util::Trace("Component 1 ", msg, hr);
}


void __stdcall CC::Fz() { trace("Fz"); }


CC::CC(IUnknown* pUnknownOuter):CUnknown(pUnknownOuter)
{
	//empty
}

CC::~CC()
{
	trace("Destroy self");
}

HRESULT __stdcall CC::NonDelegatingQueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IZ)
	{
		return FinishQI(static_cast<IZ*>(this), ppv);
	}
	else
	{
		return CUnknown::NonDelegatingQueryInterface(iid, ppv);
	}
}


HRESULT CC::CreateInstance(IUnknown*pUnknownOuter, CUnknown**ppNewComponent)
{
	*ppNewComponent = new CC(pUnknownOuter);
	return S_OK;
}








