#include "ContainmentOuterServer.h"
#include "Util.h"
#include "RegisterAPI.h"

static inline void trace(const char* msg)
{
	Util::Trace("ContainmentOuterServer", msg, S_OK);
}
static inline void trace(const char* msg, HRESULT hr)
{
	Util::Trace("ContainmentOuterServer", msg, hr);
}

CSumSubtract::CSumSubtract() :m_cRef(1)
{
	m_pIMul = NULL;
	m_pIDiv = NULL;
	InterlockedIncrement(&g_cComponents);
}

CSumSubtract::~CSumSubtract()
{
	if (m_pIMul)
	{
		m_pIMul->Release();
	}
	if (m_pIDiv)
	{
		m_pIDiv->Release();
	}

	InterlockedDecrement(&g_cComponents);
	trace("Destroy Self");
}

HRESULT __stdcall CSumSubtract::QueryInterface(REFIID iid, void **ppvObject)
{
	if (iid == IID_IUnknown)
	{
		*ppvObject = static_cast<ISum*>(this);
	
	}
	else if (iid == IID_ISum)
	{
		*ppvObject = static_cast<ISum*>(this);
	}
	else if (iid == IID_ISubtract)
	{
		*ppvObject = static_cast<ISubtract*>(this);
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

ULONG __stdcall CSumSubtract::AddRef(void)
{
	InterlockedIncrement(&m_cRef);
	return m_cRef;

}

ULONG __stdcall CSumSubtract::Release(void)
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete(this);
		return 0;
	}
	return m_cRef;
}

void __stdcall CSumSubtract::SumOfTwoNumbers(int n1, int n2, int *n3)
{
	
	*n3 = n1 + n2;

}


void __stdcall CSumSubtract::SubtractionOfTwoNumbers(int n1, int n2, int *n3)
{

	*n3 = n1 - n2;
}

void __stdcall CSumSubtract::MultiplicationOfTwoNumbers(int n1, int n2, int *n3)
{

	m_pIMul->MultiplicationOfTwoNumbers(n1, n2, n3);
}

void __stdcall CSumSubtract::DivisionOfTwoNumbers(int n1, int n2, int *n3)
{
	m_pIDiv->DivisionOfTwoNumbers(n1, n2, n3);
}

HRESULT __stdcall CSumSubtract::InitializeInnerComponent(void)
{
	HRESULT hr;

	hr = CoCreateInstance(CLSID_CMultiplyDivide, NULL, CLSCTX_INPROC_SERVER, IID_IMultiply, (void**)&m_pIMul);
	if (FAILED(hr))
	{
		return E_NOINTERFACE;
	}

	hr = m_pIMul->QueryInterface(IID_IDivide, (void**)&m_pIDiv);
	if (FAILED(hr))
	{
		return E_NOINTERFACE;
	}

	return S_OK;
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

	CSumSubtract *pCSumSubtract = new CSumSubtract;

	if (pCSumSubtract == NULL)
	{
		return E_OUTOFMEMORY;
	}


	HRESULT hr;
	//Intialize inner component
	hr = pCSumSubtract->InitializeInnerComponent();
	if (FAILED(hr))
	{
		trace("InitializeInnerComponent", hr);
	}

	//Get the requested interface
	hr = pCSumSubtract->QueryInterface(iid, ppvObject);
	
	if (FAILED(hr))
	{
		trace("CreateInstance", hr);
	}
	//decrement the count increased by the constructor
	pCSumSubtract->Release();

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

	if (clsid != CLSID_CSumSubtract)
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
	return RegisterServer(g_hModule, CLSID_CSumSubtract, g_szFriendlyName, g_szVerIndProgID, g_szProgID);
}

STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_CSumSubtract, g_szVerIndProgID, g_szProgID);
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














