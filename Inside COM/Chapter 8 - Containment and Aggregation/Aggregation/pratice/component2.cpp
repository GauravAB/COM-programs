#include <windows.h>
#include <iostream>
#include "interface.h"


static long g_cComponents = 0;
static long g_cServerLocks = 0;
static HMODULE g_hModule = NULL;


/*
struct NonDelegatingIUnknown
{
	virtual HRESULT __stdcall QueryInterface(const IID &iid , void **ppv) =0;
	virtual ULONG __stdcall AddRef(void) = 0 ;
	virtual ULONG __stdcall Release(void) = 0;
};
*/




class CB : public IY
{
public:
	
	CB();
	~CB();

	virtual HRESULT __stdcall QueryInterface(const IID &iid , void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	virtual void __stdcall Fy() { std::cout << "Fy called " <<std::endl;}

private:
	long m_cRef;
};


CB::CB()
{
	m_cRef = 1;
	InterlockedIncrement(&g_cComponents);
}

CB::~CB()
{
	InterlockedDecrement(&g_cComponents);
}


HRESULT __stdcall CB::QueryInterface(const IID &iid , void **ppv)
{
	if(iid == IID_IUnknown)
	{
		*ppv = static_cast<IY*>(this);
	}
	else if(iid == IID_IY)
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



ULONG __stdcall CB::AddRef()
{
	return (InterlockedIncrement(&m_cRef));
}

ULONG __stdcall CB::Release()
{
	if(InterlockedDecrement(&m_cRef) == 0)
	{
		delete(this);
		return 0;
	}

	return m_cRef;
}


class CFactoryB : IClassFactory
{
	public:
		CFactoryB()
		{
			m_cRef = 1;
		}

		~CFactoryB()
		{
			//
		}
		HRESULT __stdcall QueryInterface(const IID &iid , void **ppv);
		ULONG __stdcall AddRef(void);
		ULONG __stdcall Release(void);

		HRESULT __stdcall CreateInstance(IUnknown *PUnknown ,const IID &iid , void **ppv);
		HRESULT __stdcall LockServer(BOOL bLock);

	private:
		long m_cRef;
};


HRESULT __stdcall CFactoryB :: QueryInterface(const IID &iid , void **ppv)
{
	if( ( iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return 0;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}


ULONG __stdcall CFactoryB :: AddRef(void)
{
	return(InterlockedIncrement(&m_cRef));
}

ULONG __stdcall CFactoryB :: Release(void)
{
	if(InterlockedIncrement(&m_cRef) == 0)
	{
		delete(this);
		return(0);
	}

	return m_cRef;
}

HRESULT __stdcall CFactoryB::CreateInstance(IUnknown*pUnknownOuter , const IID &iid , void **ppv)
{
	if(pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	CB *pCB = new CB;
	
	if(pCB == NULL)
		{
			return E_OUTOFMEMORY;
		}

	HRESULT hr = pCB->QueryInterface(iid,ppv);

	if(FAILED(hr))
	{
		return E_NOINTERFACE;
	}

	pCB->Release();

	return S_OK;
}



HRESULT __stdcall CFactoryB::LockServer(BOOL bLock)
{
	if(bLock)
	{
		InterlockedIncrement(&g_cServerLocks);
	}
	else
	{
		InterlockedDecrement(&g_cServerLocks);
	}

	return TRUE;
}


STDAPI DllGetClassObject(const CLSID &clsid , const IID &iid , void **ppv)
{
	if(clsid != CLSID_CB)
	{

		return CLASS_E_CLASSNOTAVAILABLE;
	}

	CFactoryB *pCFactory = new CFactoryB;
	if(pCFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	HRESULT hr = pCFactory->QueryInterface(iid,ppv);
	
	if(FAILED(hr))
	{
		return E_NOINTERFACE;
	}

	pCFactory->Release();

	return S_OK;
}

STDAPI DllCanUnloadNow()
{
	if( (g_cComponents ==0) && (g_cServerLocks ==0))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}



BOOL APIENTRY DllMain(HANDLE hModule , DWORD dwReason , void *lpReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hModule;
	}

	return TRUE;	
}











