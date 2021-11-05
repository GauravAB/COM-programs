#include "iface.h"
#include "util.h"
#include "CUnknown.h"
#include "component1.h"
#include <iostream>

static inline void trace(char * msg)
{
	Util::Trace("Component1:", msg, S_OK);
}

static inline void trace(char*msg, HRESULT hr)
{
	Util::Trace("Component1:", msg, hr);
}

HRESULT __stdcall CA::FxStringIn(wchar_t* szIn)
{
	//Display the incoming string
	std::cout << "FxStringIn received a string " << szIn << std::endl;
	return S_OK;
}

HRESULT __stdcall CA::FxStringOut(wchar_t** pszOut)
{
	const wchar_t wsz[] = L"[String from FxStringOut]";
	const int iLength = (wcslen(wsz) + 1) * sizeof(wchar_t);

	//allocate an outgoing string

	wchar_t* pBuf = static_cast<wchar_t*>(::CoTaskMemAlloc(iLength));
	if (pBuf == NULL)
	{
		return E_OUTOFMEMORY;
	}

	//Copy string from output buffer
	wcscpy(pBuf, wsz);

	*pszOut = pBuf;

	return S_OK;
}


CA::CA(IUnknown* pUnknownOuter) : CUnknown(pUnknownOuter),m_pUnknownInner(NULL),m_pIY(NULL)
{
	//Empty
}


//Destructor
CA:: ~CA()
{
	trace("Destroy Self.");
}

//NonDelegating QueryInterface implementation

HRESULT __stdcall CA::NondelegatingQueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IX)
	{
		return FinishQI(static_cast<IX*>(this), ppv);
	}
	else if (iid == IID_IY)
	{
		trace("return IY component of aggregated component");
		return m_pUnknownInner->QueryInterface(iid, ppv);
	}
	else if (iid == IID_IZ)
	{
		trace("return IZ interface of inner aggregated component");
		return m_pUnknownInner->QueryInterface(iid, ppv);
	}
	else if (iid == IID_IMarshal)
	{
		trace("the com library asked for IMarshal");
		//we dont implement it
		return CUnknown::NondelegatingQueryInterface(iid, ppv);
	}
	else
	{
		return CUnknown::NondelegatingQueryInterface(iid, ppv);
	}
}


//Initialze the component by creating the contained component

HRESULT CA::InitializeInnerComponent()
{
	trace("Create Component2 , which is aggregated.");
	HRESULT hr = CoCreateInstance(CLSID_Component2, GetOuterUnknown(), CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&m_pUnknownInner);
	
	if (FAILED(hr))
	{
		trace("could not create inner component", hr);
		return E_FAIL;
	}

	trace("get pointer to interface IY for cache");
	hr = m_pUnknownInner->QueryInterface(IID_IY, (void**)&m_pIY);
	
	if (FAILED(hr))
	{
		trace("inner component does not support IY", hr);
		m_pUnknownInner->Release();
		m_pUnknownInner = NULL;
		return E_FAIL;
	}

	
	trace("Got IY interface pointer . Release reference.");
	GetOuterUnknown()->Release();
	return S_OK;
}

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


HRESULT CA::CreateInstance(IUnknown* pUnknownOuter,CUnknown** ppNewComponent)
{
	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	*ppNewComponent = new CA(pUnknownOuter);
	return S_OK;
}












