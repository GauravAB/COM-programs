#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <interface.h>


static HMODULE g_hModule = NULL;
static long g_cComponents = 0;
static long g_cNumberOfServices = 0;


void trace(const char msg) {std::cout<<msg<<std::endl;}

//outer component
class CA : public IX
{
public:
	virtual HRESULT __stdcall QueryInterface(const IID &iid , void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	virtual void Fx() { std::cout<< "Fx called" <<std::endl;}


	//class constrcutor and destructor
	CA();
	~CA();


	//function to initialize the inner Component
	HRESULT __stdcall InitializeInnerComponent();
private:
	long m_cRef;
	IY* m_pIY;
	IUnknown *m_pUnknownInner;
};


CA :: CA()
{
	m_cRef = 1;
	m_pUnknownInner = NULL;
	InterlockedIncrement(&m_cRef);
}

CA :: ~CA()
{
	InterlockedDecrement(&g_cComponents);
	trace("Destroy Self");

	m_cRef = 1;

	IUnknown *m_pUnknownOuter = this;
	m_pUnknownOuter->AddRef();

	if(m_pIY != NULL)
	{
		m_pIY->Release();
	}

	if(m_pUnknownInner != NULL)
	{
		m_pUnknownInner->Release();
	}

}

HRESULT __stdcall CA::InitializeInnerComponent()
{
	IUnknown *pUnknownOuter = this;
	trace("create inner component");

	HRESULT hr;

	//get inner component Iunknown
	hr = CoCreateInstance(CLSID_Component2,pUnknownOuter,CLSCTX_INPROC_SERVER,IID_IUnknown,(void**)&m_pUnknownInner);

	if(FAILED(hr))
	{
		trace("could not create component");
		return E_FAIL;
	}

	hr = m_pIUnknownInner->QueryInterface(IID_IY,(void**)m_pIY);

	if(FAILED(hr))
	{
		trace("inner component does not support inner interface");
		m_pIUnknownInner->Release();
		m_pIUnknownInner = NULL;
		m_pIY = NULL;


		return E_FAIL;

	}

	pUnknownOuter->Release();

	return S_OK;
}


HRESULT __stdcall CA::QueryInterface(const IID &iid , void **ppv)
{
	if(iid == IID_IUnknown)
	{
		*ppv = static_cast<IUnknown*>(this);
	}
	else if(iid == IID_IX)
	{
		*ppv = static_cast<IX*>(this);
	}
	else if(iid == IID_IY)
	{
		trace("Return inner components IY");
		return(m_pUnknownInner->QueryInterface(iid,ppv));
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













class CFactory :: public IClassFactory
{
public:

	virtual HRESULT __stdcall QueryInterface(const IID &iid , void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	virtual HRESULT __stdcall CreateInstance(IUnknown *pUnknownOuter ,const IID &iid , void **ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);

	CFactory(): m_cRef(0) {}
	~CFactory(){}


private:
		long m_cRef;
};


HRESULT __stdcall CFactory :: QueryInterface(const IID &iid, void **ppv)
{
	
	if((iid == IID_IUnknown ) || (iid == IID_IClassFactory))
	{
		pI = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return S_OK;
}


ULONG __stdcall CFactory :: Release()
{
	long temp = InterlockedDecrement(&m_cRef);

	if(temp == 0)
	{
		delete(this);
		return 0;
	}

	return m_cRef;

}


ULONG __stdcall CFactory :: AddRef()
{
	return InterlockedIncrement(&m_cRef);
}


HRESULT _stdcall CFactory :: CreateInstance(IUnknown* pUnknownOuter, const IID &iid , void **ppv)
{
	trace("CreateInstance reaching here");
	//cannot aggregate

	if(pUnknownOuter != NULL)
	{

		return CLASS_E_NOAGGREGATION;
	}
 
	CA *pCA = new CA;

	if(pCA == NULL)
	{
		return E_OUTOFMEMORY;
	}

	HRESULT hr = pCA->InitializeInnerComponent();

	if(FAILED(hr))
	{
		pCA->Release();
		return hr;
	}

	//get the requested interface

	hr = pCA->QueryInterface(iid,ppv);

	pCA->Release();

	return hr;
}


HRESULT __stdcall CFactory :: LockServer(BOOL bLock)
{
	if(bLock)
	{
		InterLockedIncrement(&g_cNumberOfServerLocks);
	}
	else
	{
		InterlockedDecrement(&g_cNumberOfServerLocks);
	}

	return S_OK;
}



STDAPI  DllCanUnloadNow()
{
	if((g_cComponents == 0) && (g_cNumberofServerLocks == 0))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}


STDAPI DllGetClassObject(const CLSID &clsid , const IID &iid , void **ppv)
{
	trace("DllGetClassObject triggered");

	if(clsid != CLSID_Component1)
	{
		return 	CLASS_E_CLASSNOTAVAILABLE;
	}

	//create class factory

	CFactory *pFactory = new CFactory;

	HRESULT hr = pFactory->QueryInterface(iid,ppv);

	pFactory->Release();

	return hr;
}



BOOL APIENTRY DllMain(HANDLE hModule , DWORD dwReasom , void *lpReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule == (HMODULE)hModule;

	}

	return true;
}


































































































