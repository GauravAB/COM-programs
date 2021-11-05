#include <Windows.h>
#include <iostream>
#include <stdlib.h>
#include "interfaces.h"
#include "registry.h"

void trace(const char*msg) { std::cout<<"component1:\t" << msg << std::endl; }

//Global variables

static HMODULE g_hModule = NULL;	//Dll module handle
static long g_cComponents = 0;		//count of active components
static long g_cServerLocks = 0;		//count of server locks

//friendly name of component
const char g_szFriendlyName[] = "Inside COM Chapter 8 containment_example , component1";

//Version independent progID
const char g_szVerIndProgID[] = "InsideCOM.Chapter8.Component1";

//ProgID
const char g_szProgID[] = "InsideCOM.Chapter8.Containment.component1.1";



//Component A

class CA : public IX, public IY
{
public:

	//IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//interface IX
	virtual void __stdcall Fx() { std::cout << "Fx call success" << std::endl; }
	
	//interface IY
	virtual void __stdcall Fy() { m_pIY->Fy();}
	
	//Constructor 
	CA();

	//Destructor 
	~CA();

	//initialization function called by classfactory
	//to create inner component
	HRESULT __stdcall InitializeInnerComponent();

private:

	//reference count
	long m_cRef;

	//Pointer to contained component IY

	IY* m_pIY;
};

CA::CA()
{
	//setting private variables
	m_cRef = 1;
	m_pIY = NULL;

	InterlockedIncrement(&g_cComponents);
}

CA :: ~CA()
{
	InterlockedDecrement(&g_cComponents);
	trace("destroy Self");

	//Release the contained component
	if (m_pIY != NULL)
	{
		m_pIY->Release();
	}
}


//Initialize the component by creating the contained component

HRESULT __stdcall CA::InitializeInnerComponent()
{
	trace("Create contained component");

	HRESULT hr = CoCreateInstance(CLSID_Component2, NULL, CLSCTX_INPROC_SERVER, IID_IY,(void**)&m_pIY);

	if (FAILED(hr))
	{
		trace("Could not create inner component");
		return(E_FAIL);
	}
	else
	{
		return S_OK;
	}
}


//IUnknown Implementation

HRESULT __stdcall CA::QueryInterface(const IID &iid, void**ppv)
{
	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IX)
	{
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IY)
	{
		*ppv = static_cast<IY*>(this);
	}
	else
	{
		*ppv = NULL;
		return (E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return (S_OK);
}

ULONG __stdcall CA::AddRef()
{
	long value;
	value = InterlockedIncrement(&m_cRef);
	std::cout << "CA: m_cRef value incremented to: " << value << std::endl;

	return m_cRef;
}


ULONG __stdcall CA::Release()
{
	long value = InterlockedDecrement(&m_cRef);
	std::cout << "CA: m_cRef value decremented to: " << value << std::endl;
	
	if (value == 0)
	{
		delete(this);
	}
	
	return m_cRef;
}


//Class Factory

class CFactory : public IClassFactory
{
public:
	
	//IUnknown methods
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void** ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//IClassFactory Interface
	virtual HRESULT __stdcall CreateInstance(IUnknown* pUnknownOuter, const IID &iid, void**ppv);
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


//Class Factory IUnknown implementation

HRESULT __stdcall CFactory::QueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<IClassFactory*>(this);

	}
	else if (iid == IID_IClassFactory)
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return (E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	
	return S_OK;

}

ULONG __stdcall CFactory::AddRef(void)
{
	long value;

	value = InterlockedIncrement(&m_cRef);
	std::cout << "CFactory: m_cRef value Incremented to " << value << std::endl;
	return(m_cRef);
}

ULONG __stdcall CFactory::Release(void)
{
	long value;

	value = InterlockedDecrement(&m_cRef);
	std::cout << "CFactory: m_cRef value decremented to " << value << std::endl;
	
	if (value == 0)
	{
		delete(this);
	}
	return (value);
}

//IClassFactory Implementations

HRESULT __stdcall CFactory :: CreateInstance(IUnknown *pUknownOuter, const IID &iid, void **ppv)
{
	//cannot aggregate

	if (pUknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	//Create Component
	CA *pA = new CA;

	if (pA == NULL)
	{
		return (E_OUTOFMEMORY);
	}

	//Initialize the inner component
	HRESULT hr = pA->InitializeInnerComponent();

	if (FAILED(hr))
	{
		pA->Release();
		return hr;
	}

	//get the requested interface 

	pA->QueryInterface(iid,ppv);

	pA->Release();	
	return hr;
}

	
//Lock Server
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
	if ((g_cComponents == 0) && (g_cServerLocks == 0))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

//Get Class factory

STDAPI DllGetClassObject(const CLSID& clsid, const IID &iid, void **ppv)
{
	//can we create this component
	if (clsid != CLSID_Component1)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	//Create Class Factory
	CFactory *pFactory = new CFactory;
	
	if (pFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}


	//Get requested interface
	HRESULT hr = pFactory->QueryInterface(iid, ppv);
	pFactory->Release();
	return hr;
}


//Server registration

STDAPI DllRegisterServer()
{
	return(RegisterServer(g_hModule, CLSID_Component1, g_szFriendlyName, g_szVerIndProgID, g_szProgID));
}

STDAPI DllUnregisterServer()
{
	return (UnregisterServer(CLSID_Component1, g_szVerIndProgID, g_szProgID));
}


//DLL module information

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void *lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hModule;
	}

	return TRUE;
}

	






































