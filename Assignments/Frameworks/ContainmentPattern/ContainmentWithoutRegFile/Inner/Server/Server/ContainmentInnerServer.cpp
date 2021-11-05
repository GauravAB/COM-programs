#include <Windows.h>
#include "ContainmentInnerServer.h"
#include <cstdio>
#include "Util.h"
#include "register.h"



//global variables

long glNumberOfActiveComponents=0;
long glNumberOfServerLocks=0;
HMODULE ghModule = NULL;



class CMulDiv : public IMultiplication , public IDivision
{	
private:
	long m_cRef;

public:
	CMulDiv();
	~CMulDiv();

	//IUnknown methods
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//Interface methods
	virtual HRESULT __stdcall multiplicationOfTwoIntegers(int, int, int*);
	virtual HRESULT __stdcall divisionOfTwoIntegers(int, int, int*);
	

};

class myClassFactory : public IClassFactory
{
private:
	long m_cRef;
public:
	//IUnknown methods
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);
	
	//class methods
	myClassFactory();
	~myClassFactory();

	//IClassFactory methods
	virtual HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	virtual HRESULT __stdcall LockServer(BOOL bLock);
};


//definations for coclass


//IUnknown definations
HRESULT __stdcall CMulDiv::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<IMultiplication*>(this);
	}
	else if (riid == IID_IMul)
	{
		*ppv = static_cast<IMultiplication*>(this);
	}
	else if (riid == IID_IDiv)
	{
		*ppv = static_cast<IDivision*>(this);
	}
	else
	{
		*ppv = NULL;
		return 0;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG __stdcall CMulDiv::AddRef(void)
{
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CMulDiv::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}

	return m_cRef;
}

//Interface definations
HRESULT __stdcall CMulDiv::multiplicationOfTwoIntegers(int n1, int n2, int* n3)
{
	*n3 = n1 * n2;
	return S_OK;
}

HRESULT __stdcall CMulDiv::divisionOfTwoIntegers(int n1, int n2, int *n3)
{
	if (n2 == 0)
	{
		*n3 = 9999999;
	}
	*n3 = n1 / n2;
	return S_OK;
}

//class constructor and destructor
CMulDiv::CMulDiv(void)
{
	m_cRef = 1;
	
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CMulDiv::~CMulDiv(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

//myClassFactory methods

myClassFactory::myClassFactory()
{
	m_cRef = 1;
}


myClassFactory::~myClassFactory()
{
	//nothing yet
}


HRESULT __stdcall myClassFactory::QueryInterface(REFIID riid, void **ppv)
{
	if ((riid == IID_IUnknown) || (riid == IID_IClassFactory))
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

ULONG __stdcall myClassFactory::AddRef()
{
	return InterlockedIncrement(&m_cRef);

}
ULONG __stdcall myClassFactory::Release()
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}	
	return m_cRef;
}


//IClassFactory Methods
HRESULT __stdcall myClassFactory::CreateInstance(IUnknown *pUnknownOuter, REFIID riid, void**ppv)
{
	CMulDiv *pCMulDiv = NULL;
	HRESULT hr;

	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	pCMulDiv = new CMulDiv;
	if (pCMulDiv == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCMulDiv->QueryInterface(riid, ppv);
	
	if (FAILED(hr))
	{
		return E_NOINTERFACE;
	}

	pCMulDiv->Release();
	return S_OK;
}

HRESULT __stdcall myClassFactory::LockServer(BOOL bLock)
{
	if (bLock)
	{
		InterlockedIncrement(&glNumberOfServerLocks);
	}
	else
	{
		InterlockedDecrement(&glNumberOfServerLocks);
	}
	return S_OK;
}

STDAPI DllCanUnloadNow(void)
{
	if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

STDAPI DllGetClassObject(const CLSID &clsid , REFIID riid , void **ppv)
{
	HRESULT hr;
	myClassFactory* pmyClassFactory = NULL;

	if (clsid != CLSID_CMulDiv)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	pmyClassFactory = new myClassFactory;
	
	if (pmyClassFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pmyClassFactory->QueryInterface(riid, ppv);
	
	if (FAILED(hr))
	{
		return E_NOINTERFACE;
	}

	pmyClassFactory->Release();

	return S_OK;
}

STDAPI DllRegisterServer()
{
	return RegisterServer(L"Containment Inner Server", ghModule, CLSID_CMulDiv);
}

STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_CMulDiv);
}



BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		ghModule = (HMODULE)hInstance;
	}
	return TRUE;
}




