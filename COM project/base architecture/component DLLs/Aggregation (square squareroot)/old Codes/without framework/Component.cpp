#include "Component.h"
#include "CFactory.h"
#include <Windows.h>
#include "Util.h"
#include "RegisterAPI.h"

static void trace(const char *msg)
{
	Util::Trace("Component CA", msg, S_OK);
}

static void trace(const char *msg, HRESULT hr)
{
	Util::Trace("CFactory CA", msg, hr);
}


//IUnknown Methods for Component
HRESULT __stdcall CA::QueryInterface(const IID &iid, void **ppv)
{
	if (iid == IUnknown)
	{
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IX)
	{
		*ppv = static_cast<IX*>(this);
	}
	//else if (iid == IID_IY)
	//{
		//aggregate
	//}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	//increment components count
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
}

ULONG __stdcall CA::AddRef(void)
{
	InterlockedIncrement(&m_cRef);

	return m_cRef;
}

ULONG __stdcall CA::Release(void)
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}

	return m_cRef;
}

//Class Constructor
CA::CA():m_cRef(1)
{
	InterlockedIncrement(&g_cComponents);
}

//Class Destructor
CA::~CA()
{
	trace("Destroy this");
	//Release inner component if you still have
	if (pUnknownInner)
	{
		pUnknownInner->Release();
		pUnknownInner = NULL;
	}
	//Release(this component)
	InterlockedDecrement(&g_cComponents);
}

//Class methods

HRESULT __stdcall CA::Fx(void)
{
	trace("Fx called");
	return S_OK;
}



//IUknown methods for CFactory implementation
HRESULT __stdcall CFactory::QueryInterface(const IID &iid, void **ppv)
{
	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	//increment components reference count
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
}

ULONG __stdcall CFactory::AddRef(void)
{
	InterlockedIncrement(&m_cRef);

	return m_cRef;
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

//CFactory methods

CFactory::CFactory(void) :m_cRef(1) {}
CFactory:: ~CFactory(void)
{
	trace("destroy this");
}



//IClassFactory Interface

HRESULT __stdcall CFactory::CreateInstance(IUnknown *pUnknownOuter, const IID &iid, void**ppv)
{
	if ((pUnknownOuter != NULL) && (iid != IID_IUnknown))
	{
		return CLASS_E_NOAGGREGATION;
	}
	
	CA *pCA = new CA;
	
	if (pCA == NULL)
	{
		return E_OUTOFMEMORY;
	}
	
	HRESULT hr = pCA->QueryInterface(iid, ppv);

	if (FAILED(hr))
	{
		trace("CreateInstance", hr);
	}
	return hr;
}


HRESULT __stdcall LockServer(BOOL bLock)
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

STDAPI DllGetClassObject(const CLSID &clsid , const IID &iid , void **ppv)
{
	if (clsid != CLSID_CA)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	//return instance of class factory

	CFactory *pCFactory = new CFactory;
		
	if (pCFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}
	HRESULT hr;
	hr = pCFactory->QueryInterface(iid, ppv);
	
	if (FAILED(hr))
	{
		trace("DllGetClassObject", hr);
	}

	return hr;
}


STDAPI DLLCanUnloadNow(void)
{
	if ((g_cComponents == 0) && (g_cServerLocks == 0))
	{
		return S_OK;
	}
	else
	{
		S_FALSE;
	}
}

STDAPI DllRegisterServer()
{
	return RegisterServer(g_hModule, CLSID_CA, g_szFriendlyName, g_szVerIndProgID, g_szProgID);
}

STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_CA, g_szVerIndProgID, g_szProgID);
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hInstance;
	}

	return TRUE;
}







