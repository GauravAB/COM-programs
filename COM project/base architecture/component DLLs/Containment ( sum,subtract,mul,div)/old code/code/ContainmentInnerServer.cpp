#include "ContainmentInnerServer.h"
#include "Util.h"
#include "RegisterAPI.h"

static inline void trace(const char* msg)
{
	Util::Trace("ContainmentInnerServer", msg, S_OK);
}
static inline void trace(const char* msg, HRESULT hr)
{
	Util::Trace("ContainmentInnerServer", msg, hr);
}

CMultiplyDivide::CMultiplyDivide() :m_cRef(1)
{
	InterlockedIncrement(&g_cComponents);
}

CMultiplyDivide::~CMultiplyDivide()
{
	InterlockedDecrement(&g_cComponents);
	trace("Destroy Self");
}

HRESULT __stdcall CMultiplyDivide::QueryInterface(REFIID iid, void **ppvObject)
{
	if (iid == IID_IUnknown)
	{
		*ppvObject = static_cast<IMultiply*>(this);
	
	}
	else if (iid == IID_IMultiply)
	{
		*ppvObject = static_cast<IMultiply*>(this);
	}
	else if (iid == IID_IDivide)
	{
		*ppvObject = static_cast<IDivide*>(this);
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	
	//increment components reference countn
	reinterpret_cast<IUnknown*>(*ppvObject)->AddRef();
	return S_OK;
}

ULONG __stdcall CMultiplyDivide::AddRef(void)
{
	InterlockedIncrement(&m_cRef);
	return m_cRef;

}

ULONG __stdcall CMultiplyDivide::Release(void)
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete(this);
		return 0;
	}
	return m_cRef;
}

void __stdcall CMultiplyDivide::MultiplicationOfTwoNumbers(int n1, int n2, int *n3)
{
	
	*n3 = n1 * n2;

}


void __stdcall CMultiplyDivide::DivisionOfTwoNumbers(int n1, int n2, int *n3)
{

	if (n2 < 0)
	{
		n2 = 1;
	}

	*n3 = static_cast<int>(n1/n2);
}

HRESULT __stdcall CFactory::QueryInterface(REFIID iid, void **ppvObject)
{
	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		*ppvObject = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppvObject)->AddRef();
	return S_OK;
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
		delete(this);
		return 0;
	}

	return m_cRef;
}

HRESULT __stdcall CFactory::CreateInstance(IUnknown *pUnknownOuter, REFIID iid, void **ppvObject)
{
	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	CMultiplyDivide *pCMultiplyDivide = new CMultiplyDivide;

	if (pCMultiplyDivide == NULL)
	{
		return E_OUTOFMEMORY;
	}


	//Get the requested interface
	HRESULT hr = pCMultiplyDivide->QueryInterface(iid, ppvObject);

	if (FAILED(hr))
	{
		trace("CreateInstance", hr);
	}
	//decrement the count increased by the constructor
	pCMultiplyDivide->Release();

	return hr;

}

HRESULT __stdcall CFactory::LockServer(BOOL fLock)
{
	if (fLock)
	{
		InterlockedIncrement(&g_cServerLocks);
	}
	else
	{
		InterlockedDecrement(&g_cServerLocks);
	}

	return S_OK;
}


//Exporting functions for the DLL

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

//Get Class Factory instance for COM 

STDAPI DllGetClassObject(const CLSID &clsid, REFIID iid, void **ppvObject)
{
	trace("DllGetClassObject: Create class factory");

	if (clsid != CLSID_CMultiplyDivide)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	//Create a ClassFactorty object
	
	CFactory *pCFactory = new CFactory;
	if (pCFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	HRESULT hr = pCFactory->QueryInterface(iid, ppvObject);
	if (FAILED(hr))
	{
		trace("DllGetClassObject", hr);
	}

	//Decrement the reference count of creation
	pCFactory->Release();
	return hr;
}

STDAPI DllRegisterServer()
{
	return RegisterServer(g_hModule, CLSID_CMultiplyDivide, g_szFriendlyName, g_szVerIndProgID, g_szProgID);
}

STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_CMultiplyDivide, g_szVerIndProgID, g_szProgID);
}


//DLL 

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void*lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hModule;
	}
	return TRUE;
}














