#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "interfaces.h"


void trace(const char *msg) { std::cout << msg << std::endl; }

static long g_cComponents =0;
static long g_cServerlocks=0;
static HMODULE g_hModule= NULL;


struct INonDelegatingIUnknown
{
	virtual HRESULT __stdcall NonDelegatingQueryInterface(const IID &iid, void **ppv) = 0;
	virtual ULONG	__stdcall NonDelegatingAddRef(void) = 0;
	virtual ULONG	__stdcall NonDelegatingRelease(void) = 0;
};

class IMathMore : public ISubtract, public INonDelegatingIUnknown
{
public:
	//delegating IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv)
	{
		return m_pUnknownOuter->QueryInterface(iid, ppv);
	}

	virtual ULONG __stdcall AddRef(void)
	{
		return m_pUnknownOuter->AddRef();
	}

	virtual	ULONG __stdcall Release(void)
	{
		return m_pUnknownOuter->Release();
	}

	//NonDelegating IUnknown
	virtual HRESULT __stdcall NonDelegatingQueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall NonDelegatingAddRef(void);
	virtual ULONG __stdcall NonDelegatingRelease(void);

	 
	virtual int __stdcall SubtractionOfTwoIntegers(int n2, int n1) { return n2 - n1; }
	
	IMathMore(IUnknown* m_pUnknownOuter);
	~IMathMore();

private:
	long m_cRef;
	IUnknown *m_pUnknownOuter;
};

IMathMore :: IMathMore(IUnknown *pUnknownOuter)
{
	m_cRef = 1;
	InterlockedIncrement(&g_cComponents);

	if (pUnknownOuter == NULL)
	{
		m_pUnknownOuter =reinterpret_cast<IUnknown*>(static_cast<INonDelegatingIUnknown*>(this));
	}
	else 
	{
		trace(" InnerComponent constructor: aggregation true : IUnknown set to outer component");
		m_pUnknownOuter = pUnknownOuter;
	}
}

IMathMore :: ~IMathMore()
{

	InterlockedDecrement(&g_cComponents);
	trace("InnerComponent:\tDestroy self");
}


HRESULT __stdcall IMathMore::NonDelegatingQueryInterface(const IID &iid, void **ppv)
{
	trace("innerComponent: NonDelegatingQueryInterface called");

	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<INonDelegatingIUnknown*>(this);
	}
	else if (iid == IID_ISubtract)
	{
		*ppv = static_cast<ISubtract*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	//this increases count on the outer  component
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return S_OK;
}


ULONG __stdcall IMathMore::NonDelegatingAddRef(void)
{

	std::cout<<"innerComponent\t m_cRef: "<< InterlockedIncrement(&m_cRef)<<std::endl;
	return (m_cRef);
}


ULONG __stdcall IMathMore::NonDelegatingRelease(void)
{

	long temp = InterlockedDecrement(&m_cRef);
    
    std::cout<<"innerComponent\t m_cRef: "<< m_cRef<<std::endl;

	if ( temp == 0)
	{
		delete(this);
		return 0;
	}

	return m_cRef;
}



class CFactory : public IClassFactory
{
public:
	//IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual  ULONG __stdcall Release(void);

	virtual HRESULT __stdcall CreateInstance(IUnknown *pUnknown, const IID &iid, void **ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);

	CFactory():m_cRef(1){}
	~CFactory() {}

private:
	long m_cRef;
};

HRESULT CFactory :: QueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IUnknown || iid == IID_IClassFactory)
	{
		trace("Cfactory QueryInterface returning IUnknown Interface");

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

ULONG CFactory :: AddRef(void)
{
	return(InterlockedIncrement(&m_cRef));
}

ULONG CFactory :: Release(void)
{
	long temp;

	temp =InterlockedDecrement(&m_cRef); 
	if ( temp == 0)
	{
		delete(this);
		return 0;
	}

	return m_cRef;
}


HRESULT __stdcall CFactory ::CreateInstance(IUnknown *pUnknownOuter, const IID &iid, void **ppv)
{
	if (pUnknownOuter != NULL && iid != IID_IUnknown)
	{
		return CLASS_E_NOAGGREGATION;
	}

	IMathMore *pIMore = new IMathMore(pUnknownOuter);

	if (pIMore == NULL)
	{
		return (E_OUTOFMEMORY);
	}
	trace("create instance: instance created");

	HRESULT hr = pIMore->NonDelegatingQueryInterface(iid, ppv);
	pIMore->NonDelegatingRelease();
	return hr;
}

HRESULT __stdcall CFactory :: LockServer(BOOL bLock)
{
	if (bLock)
	{
		InterlockedIncrement(&g_cServerlocks);
	}
	else
	{
		InterlockedDecrement(&g_cServerlocks);
	}

	return S_OK;
}


STDAPI DllGetClassObject(const CLSID &clsid, const IID &iid, void **ppv)
{
	if (clsid != CLSID_CMathMore)
	{
		trace("DllGetClassObject : failed to math CLSID");
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


STDAPI DllCanUnloadNow(void)
{
	if ((g_cComponents == 0) && (g_cServerlocks == 0))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}



BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void* lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hModule;
	}

	return TRUE;
}










