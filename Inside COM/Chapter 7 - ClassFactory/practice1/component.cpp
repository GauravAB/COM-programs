#include <Windows.h>
#include <iostream>
#include "interface.h"
#include <stdlib.h>


void trace(const char * msg) { std::cout << msg << std::endl; }

//global variables

static HMODULE g_hModule = NULL;		//dll module
static long g_cComponents = 0;			//count of active components
static long g_cServerlocks = 0;			//count of locks 




//component class

class CA : public IX, public IY
{
public:
	//IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//interfaces  
	virtual void __stdcall Fx() { std::cout << "Fx called" << std::endl; }
	virtual void __stdcall Fy() { std::cout << "Fy called" << std::endl; }

	//constructor
	CA() : m_cRef(0) { InterlockedIncrement(&g_cComponents); }

	//destructor
	~CA() { InterlockedDecrement(&g_cComponents); trace("component: destroy self." };

private:
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
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);


	//Interfaces from Iclassfactory
	virtual HRESULT __stdcall CreateInstance(IUnknown *pUnknownOuter, const IID &iid, void **ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);

	//constructor
	CFactory() : m_cRef(1) {}
	~CFactory() : {trace("Class factory: \t\t Destroy self"); }
private:
	long m_cRef;
};


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


ULONG virtual __stdcall CFactory::AddRef(void)
{
	return(InterlockedIncrement(&m_cRef));
}

ULONG virtual __stdcall CFactory::Release(void)
{
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete(this);
		return 0;
	}

	return m_cRef;
}


HRESULT __stdcall CFactory :: CreateInstance(IUnknown* pUnknownOuter , const IID &iid , void **ppv)
{
	trace("CreateInstance: \t\t create component");

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

	HRESULT hr = pA->QueryInterface(iid, ppv);


	pA->Release();
	return hr;
}


HRESULT __stdcall CFactory::LockServer(BOOL bLock)
{
	if (bLock)
	{
		InterlockedIncrement(&g_cServerlocks);
	}
	else
	{
		InterLockedDecrement(&g_cServerlocks);
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


STDAPI DllGetClassObject(const CLSID &clsid, const IID &iid, void **ppv)
{
	trace("DllGetClassObeject: \t Create Class factory");

	if (clsid != CLSID_Component1)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	//Create class factory

	CFactory *pCFactory = new CFactory;

	if (pCFacory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	HRESULT hr = pCFactory->QueryInterface(iid, ppv);
	pCFactory->Release();

	return hr;
}


BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void *lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HANDLE)hModule;
	}

	return TRUE;
}




































 