 #include <Windows.h>
#include <stdlib.h>
#include <iostream>
#include "registry.h"
#include "iface.h"

void trace(const char *msg) { std::cout << "component2: " << msg << std::endl; }

//globals
static HMODULE g_hModule = NULL;
static long g_cComponents = 0;
static long g_cServerLocks = 0;

//strings

const char g_szFriendlyName[] = "Inside COM Chapter 8 Aggregation component2";
const char g_szVerIndProgID[] = "InsideCOM.Chapter8.Aggregation.component2";
const char g_szProgID[] = "InsideCOM.Chapter8.Aggregation.component2.1";


struct INondelegatingUnknown
{
	virtual HRESULT __stdcall NondelegatingQueryInterface(const IID &iid, void **ppv) = 0;
	virtual ULONG __stdcall NondelegatingAddRef(void) = 0;
	virtual ULONG __stdcall NondelegatingRelease(void) = 0;
};


//Component B

class CB : public IY , public INondelegatingUnknown
{
public:

	//delegating IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void**ppv)
	{
		trace("delegating IUnknown");
		return m_pUnknownOuter->QueryInterface(iid, ppv);
	}
	virtual ULONG __stdcall AddRef(void)
	{
		trace("Delegating AddRef");
		return m_pUnknownOuter->AddRef();
	}

	virtual ULONG __stdcall Release(void)
	{
		trace("Delegating Release");
		return m_pUnknownOuter->Release();
	}


	//NonDelegating IUnknown

	virtual HRESULT __stdcall NondelegatingQueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall	NondelegatingAddRef();
	virtual ULONG __stdcall NondelegatingRelease();

	//Interface IY
	virtual void __stdcall Fy() { std::cout << "Fy call success" << std::endl; }

	//constructor
	CB(IUnknown *m_pUnknownOuter);

	//destructor
	~CB();

private:
	long m_cRef;
	IUnknown *m_pUnknownOuter;

};


//IUnknown Implementation

HRESULT __stdcall CB::NondelegatingQueryInterface(const IID &iid, void **ppv)
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

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return S_OK;
}


ULONG __stdcall CB::NondelegatingAddRef()
{
	return (InterlockedIncrement(&m_cRef));
}


ULONG __stdcall CB::NondelegatingRelease()
{
	if ((InterlockedDecrement(&m_cRef)) == 0)
	{
		delete(this);
		return 0;
	}

	return m_cRef;
}


//Constructor

CB::CB(IUnknown* pUnknownOuter)
{
	m_cRef = 1;
	InterlockedIncrement(&g_cComponents);

	if (pUnknownOuter == NULL)
	{
		trace("Not Aggregating, delegate to nondelegating IUnknown.");
		m_pUnknownOuter = reinterpret_cast<IUnknown*>(static_cast<INondelegatingUnknown*>(this));
	}
	else
	{

		trace("Aggregating , delegate to pUnknownOuter");
		m_pUnknownOuter = pUnknownOuter;
	}
}


//Destructor

CB :: ~CB()
{
	InterlockedDecrement(&g_cComponents);
	trace("Destroy Self");
}


//Class Factory

class CFactory : public IClassFactory
{
public:
	//IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID & iid, void **ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	//IClassFactory Interface
	virtual HRESULT __stdcall CreateInstance(IUnknown *pUnknownOuter, const IID &iid, void **ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);

	//Constructor 
	CFactory()
	{
		m_cRef = 1;
	}

	//Destructor

	~CFactory()
	{
		//nothing
	}

private:
	long m_cRef;
};

//ClassFactory IUnknown implementation

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

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}


ULONG __stdcall CFactory::AddRef()
{
	return (InterlockedIncrement(&m_cRef));
}

ULONG __stdcall CFactory::Release()
{
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete(this);
	}

	return (m_cRef);
}



//IClassFactory Implementation

HRESULT __stdcall	CFactory::CreateInstance(IUnknown *pUnknownOuter, const IID &iid, void **ppv)
{
	//Aggregate only if requested IID is IID_IUnknown

	if ((pUnknownOuter != NULL) && (iid != IID_IUnknown))
	{
		return CLASS_E_NOAGGREGATION;
	}

	//Create Component
	CB *pB = new CB(pUnknownOuter);
	
	if(pB == NULL)
	{
		return E_OUTOFMEMORY;
	}

	//Get the requested interface
	HRESULT hr = pB->NondelegatingQueryInterface(iid, ppv);

	pB->NondelegatingRelease();
	return hr;
}

//LockServer
	
HRESULT __stdcall CFactory::LockServer(BOOL bLock)
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


//Exported Functions


STDAPI DllCanUnloadNow()
{
	if ((g_cComponents == 0) && (g_cServerLocks) == 0)
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}


//Get Class Factory

STDAPI DllGetClassObject(const CLSID &clsid, const IID &iid, void **ppv)
{
	if (clsid != CLSID_Component2)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	//Create class factory

	CFactory *pFactory = new CFactory;

	if (pFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	//Get required Interface

	HRESULT hr = pFactory->QueryInterface(iid, ppv);
	pFactory->Release();

	return hr;
}


STDAPI DllRegisterServer()
{
	return RegisterServer(g_hModule,CLSID_Component2, g_szFriendlyName, g_szVerIndProgID, g_szProgID);
}


STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_Component2, g_szVerIndProgID, g_szProgID);
}

//DLL module information

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void *lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule =(HMODULE) hModule;
	}

	return TRUE;
}


































