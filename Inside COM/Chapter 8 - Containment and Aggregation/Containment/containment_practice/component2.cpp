#include <windows.h>
#include <interface.h>
#include <stdlib.h>

void trace(const char *msg) {std::cout << msg << std::endl;}

static HMODULE hModule = NULL;
static long g_cComponents = 0;
static long g_cServerLocks = 0;

struct INonDelegatingUnknown
{
	virtual HRESULT __stdcall NonDelegatingQueryInterface(const IID &iid , void **ppv);
	virtual ULONG  __stdcall NonDelegatingAddRef(void);
	virtual ULONG __stdcall NonDelegatingRelease(void);


};


class CB : public IY ,public INonDelegatingUnknown
{
public:

	//delegating IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID &iid , void **ppv)
	{
		m_pUnknownOutter->QueryInterface(iid,ppv);
	}
	
	virtual ULONG __stdcall AddRef(void)
	{
		m_pUnknownOutter->AddRef();
	}
	
	virtual ULONG __stdcall Release(void)
	{
		m_pUnknownOutter->Release();
	}

	//Nondelegating IUnknown
	 virtual HRESULT NondelegatingQueryInterface(const IID &iid ,void **ppv);
	 virtual ULONG NondelegatingRelease(void);
	 virtual ULONG NondelegatingAddref(void);

	 CB(IUnknown *m_pUnknownOuter);
	 ~CB();
private:
	long m_CRef;
	IUnknown m_pUnknownOuter;
}

//IUnknown Inner non delegating

HRESULT __stdcall CB:: NondelegatingQueryInterface(const IID &iid , void **ppv)
{
	if(iid == IID_IUnknown)
	{
		*ppv = static_cast<INonDelegatingUnknown*>(this);
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


ULONG __stdcall CB :: NondelegatingAddRef()
{
	return (InterlockedIncrement(&m_cRef));
}

//constructor

CB:: CB(IUnknown* pUnknownOuter)
{
	m_cRef = 1;
	InterlockedIncrement(&g_cComponents);

	if(pUnknownOuter == NULL)
	{
		m_pUnknownOuter = reinterpret_cast<IUnknown*>(static_cast<INonDelegatingUnknown*>(this));
	}
	else
	{
		m_pUnknownOuter = pUnknownOuter;
	}
}


//Destructor

CB:: ~CB()
{
	InterlockedDecrement(&g_cComponents);
}


//Class Factory

class CFactory :: public IClassFactory
{
public:
	virtual HRESULT  __stdcall QueryInterface(const IID &iid , void **ppv);
	virtual ULONG  __stdcall Release(void);
	virtual ULONG __stdcall AddRef(void);


	virtual HRESULT __stdcall CreateInstance(IUnknown *pUnknownOuter, const IID &iid , void **ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);

	//constructor 

	CFactory()
	{
		m_cRef =1 ;
	}


	//Destructor
	~CFactory()
	{
		//
	}
private:
	long m_cRef;	
}



HRESULT  __stdcall CFactory::QueryInterface(const IID &iid , void **ppv)
{
	if( (iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
}

ULONG __stdcall CFactory :: AddRef()
{

	return (InterlockedIncrement(&m_cRef));
}

ULONG __stdcall CFactory :: Release()
{
	if(InterlockedDecrement(&m_cRef) == 0)
	{
		delete(this);
	}

	return (m_cRef);
}



//IClassFactory Implementation

HRESULT __stdcall CFactory :: CreateInstance(IUnknown* pUnknownOuter , const IID &iid , void **ppv)
{
	//aggregate only if requested IID is IID_IUnknown
	if((pUnknownOuter != NULL ) && (iid != IID_IUnknown))
	{
		return E_CLASS_NOAGGREGATION;
	}

	CB *pB = new CB(pUnknownOuter);

	if(pB == NULL)
	{
		return E_OUTOFMEMORY;
	}

	//get the requested interface
	HRESULT hr = pB->QueryInterface(iid,ppv);

	pB->NonDelegatingRelease();

	return hr;
}


HRESULT __stdcall CFactory :: LockServer(BOOL bLock)
{
	if(	bLock)
	{
		InterlockedIncrement(&g_cServerLocks);
	}
	else
	{
		InterlockeDecrement(&g_cServerLocks);
	}
}

//Exported function

STDAPI DllCanUnloadNow()
{
	if ( (g_cComponents ==0 ) && (g_cServerLocks == 0))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}

}


//Get class object
STDAPI DllGetClassObject(const CLSID &clsid , const IID &iid , void **ppv)
{
	if(clsid != CLSID_Component2)
		{
			return CLASS_E_CLASSNOTAVAILABLE;
		}

		//CREATE CLASS FACTORY
		
	CFactory *pCFactory = new CFactory;

	if(pCFactory == NULL)
	{
		return NO_OUTOFMEMORY;
	}

	//Get requiried interface

	HRESULT hr = pFactory->QueryInterface(iid,ppv);

	pFactory->Release();

	return hr;

}


BOOL APIENTRY DllMain(HANDLE hModule , DWORD  dwReason , void *lpReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hModule;

	}

	return TRUE;
}






































+








