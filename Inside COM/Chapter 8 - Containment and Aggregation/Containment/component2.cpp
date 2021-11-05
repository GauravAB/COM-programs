#include <windows.h>
#include "interfaces.h"
#include "registry.h"
#include <iostream>


void trace(const char *msg) { std::cout << "component 2 : \t" << msg << std::endl; }

//Global variables

//static variables

static HMODULE g_hModule = NULL;	//Dll module handle
static long g_cComponents = 0;		//Count of active components
static long g_cServerLocks = 0;		//Count of locks


//Friendly name of component

const char g_szFriendlyName[] = "Inside COM , Chapter 8 , Containment , Component 2";

//Version independent progID
const char g_szVerIndProgID[] = "InsideCOM.Chapter8.Containment.component2";

//ProgID
const char g_szProgID[] = "InsideCOM.Chap08.Containment.Component2.1";



//Component B

class CB : public IY
{
public:

	//IUnknown 

	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	//Interface IY
	
	virtual void __stdcall Fy() { std::cout << "FY call success" << std::endl; }

	//Constructor 

	CB();

	//Destructor
	~CB();
	
private:
	//Reference count
	long m_cRef;
};


//IUnknown Implementation

HRESULT __stdcall CB::QueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IUnknown)
	{
		*ppv = this;
	}
	else if (iid == IID_IY)
	{
		*ppv = static_cast<IY*>(this);
	}
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return(S_OK);
}

ULONG __stdcall CB::AddRef()
{
	long value;

	value = InterlockedIncrement(&m_cRef);
	std::cout << "CB: m_cRef value incremented to : " << value << std::endl;
	return(value);

}

ULONG __stdcall CB::Release()
{
	long value;

	value = InterlockedDecrement(&m_cRef);
	std::cout << "CB: m_cRef value decremented to : " << value << std::endl;

	if (value == 0)
	{
		delete(this);
	}

	return(value);

}

CB::CB()
{
	m_cRef = 1;
	InterlockedIncrement(&g_cComponents);
}


CB:: ~CB()
{
	InterlockedDecrement(&g_cComponents);
	trace("CB: destroy self");
}


class CFactory : public IClassFactory
{
public:
	//IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	//IClassFactory
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
		//empty
	}

private:
	long m_cRef;
};


//Class factory	IUnknown Implementation

HRESULT __stdcall CFactory::QueryInterface(const IID &iid, void **ppv)
{
	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return(S_OK);
}

ULONG __stdcall CFactory::AddRef()
{
	long value;

	value = InterlockedIncrement(&m_cRef);
	std::cout << "CFactory: m_cRef value incremented to : " << value << std::endl;

	return(value);
}

ULONG __stdcall CFactory::Release()
{
	long value;

	value = InterlockedDecrement(&m_cRef);
	std::cout << "CFactory: m_cRef value decremented to : " << value << std::endl;

	if (value == 0)
	{
		delete(this);
	}

	return(value);

}

HRESULT __stdcall CFactory::CreateInstance(IUnknown *pUnknownOuter, const IID &iid, void **ppv)
{
	if (pUnknownOuter != NULL)
	{
		return(CLASS_E_NOAGGREGATION);
	}

	CB *pCB = new CB;

	if (pCB == NULL)
	{
		return(E_OUTOFMEMORY);
	}

	HRESULT hr = pCB->QueryInterface(iid, ppv);

	if (FAILED(hr))
	{
		return(E_NOINTERFACE);
	}
	
	pCB->Release();
	
	return(hr);
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


//Exported functions


STDAPI DllCanUnloadNow()
{
	if ((g_cComponents = 0) && (g_cServerLocks == 0))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

STDAPI DllGetClassObject(const CLSID &clsid, const IID &iid, void **ppv)
{
	//can we create this component ?

	if (clsid != CLSID_Component2)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	//Create Class Factory
	CFactory *pFactory = new CFactory;

	if (pFactory == NULL)
	{
		return(E_OUTOFMEMORY);
	}
	
	HRESULT hr = pFactory->QueryInterface(iid, ppv);
	pFactory->Release();

	return hr;
}


STDAPI DllRegisterServer()
{
	return RegisterServer(g_hModule, CLSID_Component2, g_szFriendlyName, g_szVerIndProgID, g_szProgID);
}


STDAPI DllUnregisterServer()
{
	return (UnregisterServer(CLSID_Component2, g_szVerIndProgID, g_szProgID));
}


//Dll module information
BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void *lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hModule;
	}
	return TRUE;
}

































