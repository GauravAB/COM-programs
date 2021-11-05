#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include"iFace.h"
#include "registry.h"

void trace(const char* msg) { std::cout << msg << std::endl; }

//global variables

static HMODULE g_hModule = NULL;	//Dll Module 
static long g_cComponents = 0;		//Count of active components
static long g_cServerLocks = 0;     //Count of locks

//Friendly name of component
const char g_szFriendlyName[] = "InsideCOM classFactory Example";
const char g_szVerIndProgID[] = "InsideCOM.chapter7_GAB";

//progID

const char g_szProgID[] = "InsideCOM.chapter7_GAB";


//component code

class CA : public IX , public IY
{
public:
	//IUnknown	
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//Interface IX
	virtual void __stdcall Fx() { std::cout << "Fx" << std::endl; }
	virtual void __stdcall Fy() { std::cout << "Fy" << std::endl; }
	
	//Constructor
	CA()
	{
		m_cRef = 1;
		InterlockedIncrement(&g_cComponents);
	}
	//Destructor
	~CA()
	{
		InterlockedDecrement(&g_cComponents);
		trace("Component: \t\t Destroy self.");
	}

private:
	//Reference count
	long m_cRef;
};

HRESULT __stdcall CA::QueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IX)
	{
		*ppv = static_cast<IX*>(this);
		trace("Component:\t\t Return pointer to IX.");
	}
	else if (iid == IID_IY)
	{
		*ppv = static_cast<IY*>(this);
		trace("Component:\t\t Return pointer to IY.");
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG __stdcall CA::AddRef(void)
{
	return(InterlockedIncrement(&m_cRef));
}

ULONG __stdcall CA::Release(void)
{
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete(this);
		return 0;
	}

	return m_cRef;
}



class CFactory : public IClassFactory
{
public:
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void**ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	//interface IClassFactory
	virtual HRESULT __stdcall CreateInstance(IUnknown *pUnknownOuter, const IID& iid, void** ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);

	//constructor
	CFactory() 
	{
		m_cRef = 1;
	}
	//destructor
	~CFactory()
	{
		trace("Class Factory: \t\t Destroy Self.");
	}

private:
	long m_cRef;
};

//Class Factory IUnknown Implementation

HRESULT __stdcall CFactory::QueryInterface(const IID &iid, void** ppv)
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

ULONG __stdcall CFactory::AddRef(void)
{
	return(InterlockedIncrement(&m_cRef));
}

ULONG __stdcall CFactory::Release(void)
{
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete (this);
		return 0;
	}

	return m_cRef;
}

//IClassFactory implementation

HRESULT __stdcall CFactory::CreateInstance(IUnknown* pUnknownOuter, const IID& iid, void **ppv)
{
	trace("Class Factory: \t\t Create Component.");

	//cannot aggregate
	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	//create component

	CA *pA = new CA;
	if (pA == NULL)
	{
		return E_OUTOFMEMORY;
	}

	//Get the requested interface
	HRESULT hr = pA->QueryInterface(iid, ppv);

	//Release the IUnknown Pointer
	// if query interface failed then the component will delete itself
	pA->Release();
	return hr;

}

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


STDAPI DllCanUnloadNow()
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

//Get Class Factory

STDAPI DllGetClassObject(const CLSID &clsid, const IID &iid, void **ppv)
{
	trace("DllGetClassObject:\t Create Class Factory");

	//Can we create this component
	if (clsid != CLSID_Component1)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	//Create class factory

	CFactory *pFactory = new CFactory;	//Reference count set to 1

	if (pFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	//Get requested interface
	HRESULT hr = pFactory->QueryInterface(iid, ppv);
	pFactory->Release();

	return hr;
}

//Server Registration

STDAPI DllRegisterServer(void)
{
	return RegisterServer(g_hModule, CLSID_Component1, g_szFriendlyName, g_szVerIndProgID, g_szProgID);
}


STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_Component1, g_szVerIndProgID, g_szProgID);
}

//Dll module information

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void* lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hModule;
	}

	return TRUE;
}







