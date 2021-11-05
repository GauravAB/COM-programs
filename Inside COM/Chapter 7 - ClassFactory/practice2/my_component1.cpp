#include <windows.h>
#include <iostream>
#include "interface.h"

void trace(const char * msg){std::cout<<msg<<std::endl;}

//globals 

static long g_cComponents =0 ;
static long g_cServerLocks =0;
static HMODULE g_hModule = NULL;


class CMath : public ISum , ISubtract
{
	//public
	public:

	CMath()
	{
		m_cRef = 1 ;
	}

	~CMath()
	{
		//
	}

	virtual HRESULT __stdcall QueryInterface(const IID &iid ,void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	virtual void __stdcall SumOfTwoIntegers(int,int,int*);
	virtual void __stdcall SubtractionOfTwoIntegers(int,int,int*);

	//private
private:
	long m_cRef;
};

class CFactory : public IClassFactory
{

	//public
public:
	virtual HRESULT __stdcall QueryInterface(const IID &iid , void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	virtual HRESULT  __stdcall CreateInstance(IUnknown* pUnknownOuter ,const IID &iid , void **ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);

	CFactory(): m_cRef(1){}
	~CFactory(){}

private:
	long m_cRef;
};




HRESULT __stdcall CMath :: QueryInterface(const IID &iid , void **ppv )
{

	if(iid == IID_IUnknown)
	{
		*ppv = static_cast<ISum*>(this);
	}
	else if(iid == IID_ISum)
	{
		*ppv = static_cast<ISum*>(this);
	}
	else if(iid == IID_ISubtract)
	{
		*ppv = static_cast<ISubtract*>(this);
	}
	else
	{
		*ppv = NULL;

		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return S_OK;
}

ULONG __stdcall CMath:: AddRef(void)
{
	return (InterlockedIncrement(&m_cRef));
}


ULONG __stdcall CMath:: Release(void)
{
	if(InterlockedDecrement(&m_cRef) == 0)
		{
			delete(this);
			return NULL;
		}

		return m_cRef;
}



void __stdcall CMath::SumOfTwoIntegers(int n1 , int n2 , int *res)
{
	*res = n1 + n2;
}


void __stdcall CMath::SubtractionOfTwoIntegers(int n1, int n2 , int *res)
{
	*res = n1 - n2;
}



HRESULT __stdcall CFactory :: QueryInterface(const IID &iid , void **ppv)
{
	if((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		trace("CFactory::QueryInterface:creating IClassFactory Interface");
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		trace("CFactory:QueryInterface: failed to get interface");
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;	
}

ULONG __stdcall CFactory :: AddRef(void)
{
	return (InterlockedIncrement(&m_cRef));

}

ULONG __stdcall CFactory :: Release(void)
{
	if(InterlockedDecrement(&m_cRef) == 0)
		{
			delete(this);

			return 0;
		}
	

	return m_cRef;
}


HRESULT __stdcall CFactory::CreateInstance(IUnknown* pUnknownOuter, const IID &iid , void **ppv)
{
		trace("CreateInstance called");

		if (pUnknownOuter != NULL)
		{
			return CLASS_E_NOAGGREGATION;
		}


		//create component

		CMath *pCMath = new CMath;

		if(pCMath == NULL)
		{
			return E_OUTOFMEMORY;
		}

		//get the requested interface

		HRESULT hr = pCMath->QueryInterface(iid,ppv);

		if(FAILED(hr))
		{
			pCMath->Release();
			return (E_NOINTERFACE);
		}

		pCMath->Release();
	
		return hr;
}

HRESULT __stdcall CFactory:: LockServer(BOOL bLock)
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

STDAPI DllCanUnloadNow()
{


	if( (g_cComponents == 0 ) && (g_cServerLocks == 0))
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
	trace("DllGetClassObject called");
	if(clsid != CLSID_CMath)
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
		trace("DllGetClassObject: failed to get IClassFactory interface");
		pCFactory->Release();
		return (E_NOINTERFACE);
	}
	
	pCFactory->Release();
	
	return hr;
}


BOOL APIENTRY DllMain(HANDLE hModule ,DWORD dwReason ,void*lpReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hModule;
	}

	return TRUE;
}

































