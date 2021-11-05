#include "AggregationWithoutRegFileInner.h"
#include "interface.h"
#include "Util.h"
#include "RegisterAPI.h"

void trace(const char* msg)
{
	Util::Trace("AggregatingInnerComponent", msg, S_OK);
}

void trace(const char* msg , HRESULT hr)
{
	Util::Trace("AggregatingInnerComponent", msg, hr);
}

//NonDelegatingUnknown Methods

HRESULT __stdcall CB::NondelegatingQueryInterface(const IID &iid, void**ppv)
{
	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<INondelegatingUnknown*>(this);
	}
	else if (iid == IID_IY)
	{
		*ppv = static_cast<IY*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<INondelegatingUnknown*>(*ppv)->NondelegatingAddRef();
	
	return S_OK;
}

ULONG  __stdcall CB::NondelegatingAddRef(void)
{
	InterlockedIncrement(&m_cRef);

	return m_cRef;
}

ULONG __stdcall CB::NondelegatingRelease(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

//class methods
CB::CB(IUnknown* pUnknownOuter):m_cRef(1)
{
	if (pUnknownOuter)
	{
		m_pIUnknownOuter = pUnknownOuter;
	}
	else
	{
		m_pIUnknownOuter = reinterpret_cast<IUnknown*>(static_cast<INondelegatingUnknown*>(this));
	}

	InterlockedIncrement(&g_cComponents);
}

CB::~CB()
{
	trace("Destroy this");
	InterlockedDecrement(&g_cComponents);
}

HRESULT __stdcall CB::Fy(void)
{
	trace("Fy called");
	return S_OK;
}

HRESULT __stdcall CB::QueryInterface(const IID &iid, void **ppv)
{
	return m_pIUnknownOuter->QueryInterface(iid, ppv);
}

ULONG __stdcall CB::AddRef(void)
{
	return m_pIUnknownOuter->AddRef();
}

ULONG __stdcall CB::Release(void)
{
	return m_pIUnknownOuter->Release();
}



//CFACTORY METHODS

CFactory::CFactory():m_cRef(1)
{
}

CFactory::~CFactory()
{
	//nothing to do
}


HRESULT __stdcall CFactory::QueryInterface(const IID &iid, void**ppv)
{
	if ((iid == IID_IClassFactory) || (iid == IID_IUnknown))
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG __stdcall CFactory::AddRef(void)
{
	return 	InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}

	return m_cRef;
}

HRESULT __stdcall CFactory::CreateInstance(IUnknown *pUnknownOuter, const IID &iid, void **ppv)
{
	CB *pCB = new CB(pUnknownOuter);

	if (pCB == NULL)
	{
		return E_OUTOFMEMORY;
	}

	trace("Create Instance");
	HRESULT hr;
	hr = pCB->NondelegatingQueryInterface(iid, ppv);

	if (FAILED(hr))
	{
		trace("Create Instance", hr);
	}

	pCB->Release();
	return hr;
}

HRESULT __stdcall CFactory:: LockServer(BOOL bLock)
{
	if (bLock)
	{
		InterlockedIncrement(&g_cServerLocks);
	}
	else
	{
		InterlockedDecrement(&g_cServerLocks);
	}

	return S_OK;
}

STDAPI DllCanUnloadNow(void)
{
	if ((g_cComponents == 0) && (g_cServerLocks == 0))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

STDAPI DllGetClassObject(const CLSID &clsid, const IID &iid, void**ppv)
{
	if (clsid != CLSID_CB)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}
	
	//get class factory instance

	CFactory *pCFactory = new CFactory;

	if (pCFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}
	trace("DllGetClassObject");
	HRESULT hr;
	hr = pCFactory->QueryInterface(iid, ppv);

	if (FAILED(hr))
	{
		trace("GetClassObject", hr);
	}
	//decrement back the counter
	pCFactory->Release();
	return hr;
}

STDAPI DllRegisterServer()
{
	return RegisterServer(g_hModule, CLSID_CB, g_szFriendlyName, g_szVerIndProgID, g_szProgID);
}

STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_CB, g_szVerIndProgID, g_szProgID);
}


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hInstance;
	}
	return TRUE;
}
















