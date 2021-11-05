#include <windows.h>
#include <iostream>
#include "interface.h"

void trace(const char* msg) {std::cout<<msg<<std::endl;}


//globals


static long g_cServerLocks=0;
static long g_cComponents=0;
static HMODULE g_hModule= NULL;



class CA : public IX ,IY
{
public:

	CA();
	~CA();

	virtual HRESULT __stdcall QueryInterface(const IID &iid , void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	virtual void __stdcall Fx() { std::cout << "Fx called"<<std::endl;}
	virtual void __stdcall Fy() {  m_pIY->Fy();}

	HRESULT __stdcall InitializeInnerComponent(void);

private:
	long m_cRef;
	IUnknown *m_pIUnknownInner;
	IY *m_pIY;
};



class CFactory : public IClassFactory
{
public:
	virtual HRESULT __stdcall QueryInterface(const IID &iid , void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual  ULONG __stdcall Release(void);

	virtual HRESULT __stdcall CreateInstance(IUnknown *pUnknownOuter , const IID &iid , void **ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);

private:
	long m_cRef;
};


CA :: CA() 
{
	m_cRef = 1;
	m_pIY = NULL;
	m_pIUnknownInner = NULL;
	InterlockedIncrement(&g_cComponents);
}

CA::~CA()
{
	InterlockedDecrement(&g_cComponents);
	trace("destroying component CA");

	if(m_pIY != NULL)
	{
		m_pIY->Release();
	}
}


HRESULT __stdcall CA::QueryInterface(const IID &iid , void ** ppv)
{
	if(iid == IID_IUnknown)
		{
		*ppv = static_cast<IX*>(this);
	}
	else if(iid == IID_IX)
	{
		*ppv = static_cast<IX*>(this);
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


ULONG __stdcall CA::AddRef(void)
{
	return(InterlockedIncrement(&m_cRef));
}

ULONG __stdcall CA::Release(void)
{
	if(InterlockedDecrement(&m_cRef) == 0)
	{
		delete(this);
		return 0;
	}

	return m_cRef;
}


HRESULT __stdcall CA::InitializeInnerComponent()
{
	trace("CA::InitializeInnerComponent");

	HRESULT hr = CoCreateInstance(CLSID_CB,NULL,CLSCTX_INPROC_SERVER,IID_IY,(void**)&m_pIY);

		if(FAILED(hr))
		{
			trace("failed to initialize inner component");
			return (E_FAIL);
		}

		return S_OK;
}


HRESULT __stdcall CFactory ::QueryInterface(const IID &iid , void **ppv)
{
	if((iid == IID_IUnknown) || (iid == IID_IClassFactory))
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



ULONG __stdcall CFactory :: AddRef(void)
{
	return(InterlockedIncrement(&m_cRef));
}


ULONG __stdcall CFactory ::Release(void)
{
	if(InterlockedDecrement(&m_cRef) == 0)
	{
		delete(this);
		return 0;
	}

	return m_cRef;
}

HRESULT __stdcall CFactory :: CreateInstance(IUnknown *pUnknownOuter , const IID &iid , void** ppv)
{
	if(pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	CA *pCA = new CA;

	if(pCA == NULL)
	{
		return(E_OUTOFMEMORY);
	}
	else
	{
		trace("component create success");
	}

	HRESULT hr = pCA->InitializeInnerComponent();


	if(FAILED(hr))
	{
		trace("CA:CreateInstance: failed to get innerComponent");
		return E_FAIL;
	}


	hr = pCA->QueryInterface(iid,ppv);

	if(FAILED(hr))
	{
		trace("CA:CreateInstance: failed to get requested interface");
		return 0;
	}

	pCA->Release();

	return S_OK;

}


HRESULT __stdcall CFactory :: LockServer(BOOL bLock)
{
	if(bLock)
	{
		InterlockedIncrement(&g_cServerLocks);
	}
	else
	{
		InterlockedDecrement(&g_cServerLocks);
	}

	return S_OK;	
}


STDAPI DllGetClassObject(const CLSID &clsid , const IID &iid , void **ppv)
{

	trace("DllGetClassObject called");

	if(clsid != CLSID_CA)
	{
		trace("DllGetClassObject: clsid match failed");
	
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	CFactory *pCFactory = new CFactory;
	
	if(pCFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	HRESULT hr = pCFactory->QueryInterface(iid,ppv);

	if(FAILED(hr))
	{
		trace("DllGetClassObject: interface search failed");
	
		return 0;
	}

	pCFactory->Release();
	return S_OK;
}


STDAPI DllCanUnloadNow(void)
{
	if( (g_cComponents == 0 ) && (g_cServerLocks ==0))
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
	trace("reaching here?");
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hModule;
	}

	return TRUE;
}














