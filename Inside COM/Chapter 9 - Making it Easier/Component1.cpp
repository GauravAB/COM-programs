#include <Windows.h>
#include "UTIL.h"
#include "Component1.h"
#include "CUnknown.h"

static void trace(const char* msg)
{
	Util::Trace("Component 1 ", msg, S_OK);
}

static void trace(const char* msg , HRESULT hr)
{
	Util::Trace("Component 1 ", msg, hr);
}



void __stdcall CA::Fx(void)
{
	trace("Fx");
}

//Constructor

CA::CA(IUnknown* pUnknownOuter):CUnknown(pUnknownOuter),m_pUnknownInner(NULL),m_pIY(NULL)
{
	///Empty / rest of the implementation is in CUnknown
}


CA::~CA()
{
	trace("Destroy self");
}
HRESULT __stdcall CA::NonDelegatingQueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IX)
	{
		return FinishQI(static_cast<IX*>(this), ppv);
	}
	else if (iid == IID_IY)
	{
		trace("return IY interface of inner aggregated component");
		return m_pUnknownInner->QueryInterface(iid, ppv);
	}
	else if (iid == IID_IZ)
	{
		//queryInterface to innerComponent
		return m_pUnknownInner->QueryInterface(iid, ppv);
	}
	else
	{
		return CUnknown::NonDelegatingQueryInterface(iid, ppv);
	}
}



//Initialize inner component to get the interface pointer

HRESULT CA::Init()
{
	trace("Create Component2 which is aggregated");

	HRESULT hr = CoCreateInstance(CLSID_Component2, GetOuterUnknown(), CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&m_pUnknownInner);
	
	if (FAILED(hr))
	{
		trace("Could not create inner component",hr);
		return E_FAIL;
	}

	trace("Get pointer to interface IY to cache");
	hr = m_pUnknownInner->QueryInterface(IID_IY, (void**)&m_pIY);
	
	if (FAILED(hr))
	{
		trace("could not create inner component",hr);
	
		m_pUnknownInner->Release();
		m_pUnknownInner = NULL;
		return(E_FAIL);
	}

	//Decrement reference count by QI
	trace("Got Interface pointer release the reference");
	GetOuterUnknown()->Release();

	return S_OK;
}


//Final Release : called before deleting this component

void CA::FinalRelease()
{
	CUnknown::FinalRelease();

	GetOuterUnknown()->AddRef();

	m_pIY->Release();

	if (m_pUnknownInner != NULL)
	{

		m_pUnknownInner->Release();
	}
}


HRESULT CA::CreateInstance(IUnknown*pUnknownOuter, CUnknown**ppNewComponent)
{
	if (pUnknownOuter != NULL)
	{	
		return CLASS_E_NOAGGREGATION;
	}

	*ppNewComponent = new CA(pUnknownOuter);

	return S_OK;
}

























