#include <Windows.h>
#include "ContainmentOuterServer.h"
#include <cstdio>
#include "Util.h"
#include "register.h"


void trace(const char*msg)
{
	Util::Trace("ContainmentOuterServer", msg, S_OK);
}

void trace(const char*msg , HRESULT hr)
{
	Util::Trace("ContainmentOuterServer", msg, hr);
}


//global variables

long glNumberOfActiveComponents=0;
long glNumberOfServerLocks=0;
HMODULE ghModule = NULL;



class CSumSub : public ISum , public ISub , public IMultiplication , public IDivision
{	
private:
	long m_cRef;
	IMultiplication* m_pIMul;
	IDivision* m_pIDiv;

public:
	CSumSub();
	~CSumSub();

	//IUnknown methods
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//Interface methods
	virtual HRESULT __stdcall additionOfTwoIntegers(int, int, int*);
	virtual HRESULT __stdcall subtractionOfTwoIntegers(int, int, int*);
	virtual HRESULT __stdcall multiplicationOfTwoIntegers(int, int, int*);
	virtual HRESULT __stdcall divisionOfTwoIntegers(int, int, int*);

	HRESULT __stdcall InitializeInnerComponent(void);
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
HRESULT __stdcall CSumSub::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<ISum*>(this);
	}
	else if (riid == IID_ISum)
	{
		*ppv = static_cast<ISum*>(this);
	}
	else if (riid == IID_ISub)
	{
		*ppv = static_cast<ISub*>(this);
	}
	else if (riid == IID_IMul)
	{
		if (m_pIMul)
		{
			*ppv = static_cast<IMultiplication*>(this);
		}
		else
		{
			*ppv = NULL;
			return 0;
		}
	}
	else if (riid == IID_IDiv)
	{
		if (m_pIDiv)
		{
			*ppv = static_cast<IDivision*>(this);
		}
		else
		{
			*ppv = NULL;
			return 0;
		}
	}
	else
	{
		*ppv = NULL;
		return 0;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG __stdcall CSumSub::AddRef(void)
{
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CSumSub::Release(void)
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
HRESULT __stdcall CSumSub::additionOfTwoIntegers(int n1, int n2, int* n3)
{
	*n3 = n1 + n2;
	return S_OK;
}

HRESULT __stdcall CSumSub::subtractionOfTwoIntegers(int n1, int n2, int *n3)
{
	
	*n3 = n1 - n2;
	return S_OK;
}

HRESULT __stdcall CSumSub::multiplicationOfTwoIntegers(int n1, int n2, int *n3)
{
	//passing the call to the contained interface
	return m_pIMul->multiplicationOfTwoIntegers(n1, n2, n3);
}


HRESULT __stdcall CSumSub::divisionOfTwoIntegers(int n1, int n2, int *n3)
{
	//passing the call to the contained interface
	return m_pIDiv->divisionOfTwoIntegers(n1, n2, n3);
}


//class constructor and destructor
CSumSub::CSumSub(void)
{
	m_cRef = 1;
	
	m_pIMul = NULL;
	m_pIDiv = NULL;

	InterlockedIncrement(&glNumberOfActiveComponents);
}

CSumSub::~CSumSub(void)
{
	if (m_pIMul)
	{
		m_pIMul->Release();
		m_pIMul = NULL;
	}

	if (m_pIDiv)
	{
		m_pIDiv->Release();
		m_pIDiv = NULL;
	}

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

HRESULT CSumSub::InitializeInnerComponent(void)
{
	HRESULT hr;

	hr = CoCreateInstance(CLSID_CMulDiv, NULL, CLSCTX_INPROC_SERVER, IID_IMul, (void**)&m_pIMul);
	if (FAILED(hr))
	{
		trace("CoCreateInstance failed",hr);
		return hr;
	}
	else
	{
		if (m_pIMul)
		{
			hr = m_pIMul->QueryInterface(IID_IDiv, (void**)&m_pIDiv);
			if (FAILED(hr))
			{
				trace("IDiv interface not found",hr);
				return hr;
			}
		}
	}

	return S_OK;
}


//IClassFactory Methods
HRESULT __stdcall myClassFactory::CreateInstance(IUnknown *pUnknownOuter, REFIID riid, void**ppv)
{
	CSumSub *pCSumSub = NULL;
	HRESULT hr;

	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	pCSumSub = new CSumSub;
	if (pCSumSub == NULL)
	{
		return E_OUTOFMEMORY;
	}

	//Initialize the inner component to anticipate the use of inner mul and div interfaces
	hr = pCSumSub->InitializeInnerComponent();
	if (FAILED(hr))
	{
		trace("InitializeInnerComponent failed");
	}

	hr = pCSumSub->QueryInterface(riid, ppv);
	
	if (FAILED(hr))
	{
		return E_NOINTERFACE;
	}

	pCSumSub->Release();
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

	if (clsid != CLSID_CSumSubtract)
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
	return RegisterServer(L"Containment Inner Server", ghModule, CLSID_CSumSubtract);
}

STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_CSumSubtract);
}



BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		ghModule = (HMODULE)hInstance;
	}
	return TRUE;
}




