#include <iostream>
#include "iface.h"
#include "util.h"
#include "CUnknown.h"
#include "component3.h"


static inline void trace(char * msg)
{
	Util::Trace("Component3:", msg, S_OK);
}

static inline void trace(char*msg, HRESULT hr)
{
	Util::Trace("Component3:", msg, hr);
}

HRESULT __stdcall CC::FzStructIn(Point3d pt)
{

	std::cout <<"struct received ("<<pt.x <<"," <<pt.y<<","<<pt.z<<")"<<std::endl;

	m_pt = pt;

	return S_OK;
}



HRESULT __stdcall CC::FzStructOut(Point3d *pt)
{
	*pt = m_pt;

	return S_OK;
}


CC::CC(IUnknown* pUnknownOuter) : CUnknown(pUnknownOuter)
{
	m_pt.x = -1.0;
	m_pt.y = -1.0;
	m_pt.z = -1.0;

}

CC::~CC()
{
	trace("Destroy self.");
}


//NonDelegating QueryInterface implementation
HRESULT __stdcall CC::NondelegatingQueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IZ)
	{
		return FinishQI(static_cast<IZ*>(this), ppv);
	}
	else
	{
		return CUnknown::NondelegatingQueryInterface(iid, ppv);
	}
}


HRESULT CC::CreateInstance(IUnknown* pUnknownOuter,CUnknown** ppNewComponent)
{

	*ppNewComponent = new CC(pUnknownOuter);
	
	return S_OK;
}















