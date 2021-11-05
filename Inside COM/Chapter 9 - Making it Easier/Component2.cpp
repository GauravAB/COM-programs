#include <windows.h>
#include "CUnknown.h"
#include "interface.h"
#include "UTIL.h"
#include "Component2.h"

static inline void trace(char *msg)
{
	Util::Trace("component2", msg, S_OK);
}
static inline void trace(char *msg , HRESULT hr)
{
	Util::Trace("component2", msg, hr);
}

//Interface IY implementation
void __stdcall CB::Fy()
{
	trace("Fy");
}

//Constructor
CB::CB(IUnknown* pUnknownOuter) :CUnknown(pUnknownOuter), m_pUnknownInner(NULL),m_pIZ(NULL)
{
	//empty because most of the stuff required here is done in CUnknown
}

CB::~CB()
{
	trace("Destroy self");
}

//NonDelegating Interface implementation

HRESULT __stdcall CB::NonDelegatingQueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IY)
	{
		return FinishQI(static_cast<IY*>(this), ppv);
	}
	else if (iid == IID_IZ)
	{
		return m_pUnknownInner->QueryInterface(iid, ppv);
	}
	else
	{
		return CUnknown::NonDelegatingQueryInterface(iid, ppv);
	}
}

HRESULT CB::Init()
{
	trace("Create Component 3 , which is again aggregated");
	HRESULT hr = CoCreateInstance(CLSID_Component3, GetOuterUnknown(), CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&m_pUnknownInner);

	if (FAILED(hr))
	{
		trace("could not create inner component.", hr);
		return E_FAIL;
	}

	trace("Get pointer to interface IZ to cache.");
	hr = m_pUnknownInner->QueryInterface(IID_IZ, (void**)&m_pIZ);
	if (FAILED(hr))
	{
		trace("inner component does not support IZ.", hr);
		m_pUnknownInner->Release();
		m_pUnknownInner = NULL;
		return E_FAIL;
	}

	//Decrement the reference count cause by QI call 
	trace("Got IZ interface pointer . Release reference.");
	GetOuterUnknown()->Release();

	return S_OK;
}


 void CB::FinalRelease()
{
	
	//call base class to increment ref count to prevent recursion
	CUnknown::FinalRelease();

	GetOuterUnknown()->AddRef();

	m_pIZ->Release();

	//Release contained component
	if (m_pUnknownInner != NULL)
	{
		m_pUnknownInner->Release();
	}
}

HRESULT CB::CreateInstance(IUnknown* pUnknownOuter, CUnknown**	ppNewComponent)
{
	 *ppNewComponent = new CB(pUnknownOuter);
	 return S_OK;
}









