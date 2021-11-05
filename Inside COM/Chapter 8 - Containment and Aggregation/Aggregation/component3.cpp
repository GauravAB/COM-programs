		
#include <Windows.h>
#include <stdlib.h>
#include <iostream>
#include "iface.h"
#include "registry.h"

void trace(const char *msg) { std::cout << "component1:\t" << msg << std::endl; }

//global variables
static HMODULE g_hModule = NULL;
static long g_cComponents = 0;
static long g_cNumberOfServerLocks = 0;

//strings

const char g_szFriendlyName[] = "Chapter 8 Aggregation , component 1";
const char g_szVerIndProdID[] = "InsideCOM.Chapter8.Aggregation.component1";
const char g_szProgId[] = "InsideCOM.Chapter8.Aggregation.component1.1";



//component A

class CA : public IX
{
public:
	//IUnknown 
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//Interface IX

	virtual void __stdcall Fx() { std::cout << "Fx call Successfull" << std::endl; }

	//class c and d
	CA();
	~CA();

	//function to initialize the inner component
	HRESULT __stdcall InitializeInnerComponent();

private:
	long m_cRef;

	//pointer to aggregated component
	IY* m_pIY;

	//pointer to inner components IUnknown

	IUnknown * m_pUnknownInner;
};



CA::CA()
{
		m_cRef = 1;
		m_pUnknownInner = NULL;
		InterlockedIncrement(&m_cRef);
}

CA :: ~CA()
{
	InterlockedDecrement(&g_cComponents);
	trace("CA : Destroy Self");

	//Prevent recursive destruction on next AddRef/ Release pair.
	m_cRef = 1;

	//Counter the pUnknownOuter->Release in the init method
	IUnknown * m_pUnknownOuter = this;
	m_pUnknownOuter->AddRef();
	
	//properly release the pointer , there might be per interface

	if (m_pIY != NULL)
	{
		m_pIY->Release();
	}

	//Release contained component
	if (m_pUnknownInner != NULL)
	{
		m_pUnknownInner->Release();
	}
}

HRESULT __stdcall CA::InitializeInnerComponent()
{
	//Get pointer to Outer IUnknown
	//since this component is not aggregated, the outer unknown component is the same as this pointer

	IUnknown *pUnknownOuter = this;
	trace("Create inner component");

	HRESULT hr = CoCreateInstance(CLSID_Component2, pUnknownOuter, CLSCTX_INPROC_SERVER, IID_IUnknown,(void**) &m_pUnknownInner);

	if (FAILED(hr))
	{
		trace("Could not create component");
		return E_FAIL;
	}

	//This call will increment the reference count	on the outer component
	
	trace("Get the IY interface from the Inner component.");
	hr = m_pUnknownInner->QueryInterface(IID_IY, (void**)&m_pIY);
	if (FAILED(hr))
	{
		trace("Inner component does not support IY interface");
		m_pUnknownInner->Release();
		m_pUnknownInner = NULL;
		m_pIY = NULL;	//just to be safe

		return E_FAIL;
	}
	//pointer passed to CoCreateInstance has its count added , we need to reduce the count here

	pUnknownOuter->Release();
	return S_OK;
}

HRESULT __stdcall CA::QueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid == IID_IX)
	{
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IY)
	{
		trace("Return inner components IY interface");

		return(m_pUnknownInner->QueryInterface(iid, ppv));
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	
	return S_OK;
}

ULONG __stdcall CA::AddRef()
{
	long temp = InterlockedIncrement(&m_cRef);

	std::cout << "component1 : CA: m_cRef->" << temp << std::endl;

	return (temp);
}

ULONG __stdcall CA::Release()
{
	long temp = InterlockedDecrement(&m_cRef);

	if (temp == 0)
	{
		delete this;
		return 0;
	}
	std::cout << "component1 : CA: m_cRef->" << temp << std::endl;

	return temp;
}


//Class factory


class CFactory : public IClassFactory
{
public:
	//IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	//IClassFactory interface

	virtual HRESULT __stdcall CreateInstance(IUnknown *pUnknownOuter, const IID &iid, void **ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);	

	//Constructor 

	CFactory()
	{
		m_cRef = 1;
	}

	~CFactory()
	{
		//nothing
	}

private:
	long m_cRef;
};


//Class factory IUnknown Implementation

HRESULT __stdcall CFactory::QueryInterface(REFIID iid, void **ppv)
{
	IUnknown *pI;

	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		pI = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	pI->AddRef();

	*ppv = pI;	
	return S_OK;
}

ULONG __stdcall	CFactory::AddRef()
{
	long temp = InterlockedIncrement(&m_cRef);

	std::cout << "Component1: CFactory: m_cRef->" << temp << std::endl;

	return temp;
}

ULONG __stdcall CFactory::Release()
{
	long temp = InterlockedDecrement(&m_cRef);
	if (temp == 0)
	{
		delete(this);
		return 0;
	}

	std::cout << "Component1: CFactory: m_cRef->" << temp << std::endl;
	return temp;
}


//IClassFactory Implementation

HRESULT __stdcall CFactory::CreateInstance(IUnknown * pUnknownOuter, const IID &iid, void **ppv)
{
	trace("CreateInstance reaching here");
	//cannot aggregate
	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	//Create Component
	CA *pA = new CA;
	if (pA == NULL)
	{
		return E_OUTOFMEMORY;
	}

	//initialize the component
	HRESULT hr = pA->InitializeInnerComponent();

	if (FAILED(hr))
	{
		pA->Release();
		return hr;
	}
	
	//Get the requested interface
	hr = pA->QueryInterface(iid, ppv);
	pA->Release();

	return hr;
}

//LockServer

HRESULT __stdcall CFactory::LockServer(BOOL bLock)
{
	if (bLock)
	{
		InterlockedIncrement(&g_cNumberOfServerLocks);
	}
	else
	{
		InterlockedDecrement(&g_cNumberOfServerLocks);
	}

	return S_OK;
}




//Exported Functions
STDAPI DllCanUnloadNow()
{
	if ((g_cComponents == 0) && (g_cNumberOfServerLocks == 0))
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
	trace("DllGetClass Object reaching here");
	//Can we create this component
	if (clsid != CLSID_Component1)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	//create class factory

	CFactory *pFactory = new CFactory;

	if (pFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	HRESULT hr = pFactory->QueryInterface(iid, ppv);
	pFactory->Release();

	return hr;
}


STDAPI	DllRegisterServer()
{
	return RegisterServer(g_hModule, CLSID_Component1, g_szFriendlyName, g_szVerIndProdID, g_szProgId);
}



STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_Component1, g_szVerIndProdID, g_szProgId);
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























































