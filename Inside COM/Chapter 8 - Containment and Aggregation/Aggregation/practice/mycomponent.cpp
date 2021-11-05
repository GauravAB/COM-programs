#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include"interfaces.h"

void trace(const char* msg) { std::cout << msg << std::endl; }

//global variables

static HMODULE g_hModule = NULL;	//Dll Module 
static long g_cComponents = 0;		//Count of active components
static long g_cServerLocks = 0;     //Count of locks

//component code

class CA : public ISum
{
public:
	//IUnknown	
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//Interface IX
	virtual int __stdcall SumOfTwoIntegers(int n1 , int n2) { std::cout << "ISum called" << std::endl; return 1; }
	
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
		*ppv = static_cast<ISum*>(this);
	}
	else if (iid == IID_ISum)
	{
		*ppv = static_cast<ISum*>(this);
		trace("Component:\t\t Return pointer to IX.");
	}
	else if (iid == IID_ISum)
	{
		*ppv = static_cast<ISum*>(this);
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
	if (clsid != CLSID_CMath)
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

//Dll module information

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void* lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hModule;
	}

	return TRUE;
}







