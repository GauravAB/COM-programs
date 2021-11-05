#include <Windows.h>
#include <stdio.h>
#include "AggregationOuterServer.h"
#include "register.h"
#include "Util.h"

void trace(const char* msg,HRESULT hr)
{
	Util::Trace("OuterServer", msg, hr);
}

void trace(const char* msg)
{
	Util::Trace("OuterServer", msg, S_OK);
}



//global variables

long glNumberOfActiveComponents = 0;
long glNumberofServerLocks = 0;
HMODULE ghModule;

class CSquare : public ISquare
{
private:
	long m_cRef;
	IUnknown* m_pUnknownInner;
public:
	//IUnknown methods
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void**ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//Class methods
	CSquare();
	~CSquare();
	HRESULT __stdcall InitializeInnerComponent(void);
	
	//Interface methods
	virtual HRESULT __stdcall SquareOfAnInteger(int, int *);
};

class ClassFactory : public IClassFactory
{
private:
	long m_cRef;
public:
	//IUnknown Methods
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void**ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//Class methods
	ClassFactory();
	~ClassFactory();

	//IClassFactory methods
	virtual HRESULT __stdcall CreateInstance(IUnknown* pUnknownOuter, REFIID riid, void**ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);
};

//Definations for class CSquare

HRESULT __stdcall CSquare::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<ISquare*>(this);
	}
	else if (riid == IID_ISquare)
	{
		*ppv = static_cast<ISquare*>(this);
	}
	else if (riid == IID_ISquareroot)
	{
		m_pUnknownInner->QueryInterface(riid, ppv);
	}
	else
	{
		*ppv = NULL;
		return 0;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG __stdcall CSquare::AddRef(void)
{
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CSquare::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}
	
	return m_cRef;
}

CSquare::CSquare()
{
	m_cRef = 1;
	m_pUnknownInner = NULL;

	InterlockedIncrement(&glNumberOfActiveComponents);
}

CSquare::~CSquare()
{
	if (m_pUnknownInner)
	{
		m_pUnknownInner->Release();
		m_pUnknownInner = NULL;
	}

	InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT __stdcall CSquare::SquareOfAnInteger(int n1, int* res)
{
	//square the number
	*res = pow(n1, 2);
	return S_OK;
}

ClassFactory::ClassFactory()
{
	m_cRef = 1;
}


ClassFactory::~ClassFactory()
{
	//Nothing yet
}

//IClassfactory methods
HRESULT __stdcall ClassFactory::QueryInterface(REFIID riid, void**ppv)
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
}

ULONG __stdcall ClassFactory::AddRef(void)
{
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall ClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete this;
	}
	return m_cRef;
}

HRESULT __stdcall CSquare::InitializeInnerComponent()
{
	HRESULT hr;
	IUnknown* pUnknownOuter = reinterpret_cast<IUnknown*>(static_cast<ISquare*>(this));

	hr = CoCreateInstance(CLSID_CSquareroot, pUnknownOuter, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&m_pUnknownInner);

	if (FAILED(hr))
	{
		trace("CoCreateInstance failed",hr);
		return E_FAIL;
	}

	return hr;
}

HRESULT __stdcall ClassFactory::CreateInstance(IUnknown* pUnknownOuter, REFIID riid, void**ppv)
{
	HRESULT hr;
	CSquare* pCSquare = NULL;

	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	pCSquare = new CSquare;
	
	if (pCSquare == NULL)
	{
		return E_OUTOFMEMORY;
	}
	
	hr = pCSquare->InitializeInnerComponent();
	
	if (FAILED(hr))
	{
		trace("InitializeInnerComponent failed");
		pCSquare->Release();
		return hr;
	}

	hr = pCSquare->QueryInterface(riid,ppv);
	
	if (FAILED(hr))
	{
		return E_NOINTERFACE;
	}

	pCSquare->Release();
	
	return hr;
}

HRESULT __stdcall ClassFactory::LockServer(BOOL bLock)
{
	if (bLock)
	{
		InterlockedIncrement(&glNumberofServerLocks);
	}
	else
	{
		InterlockedDecrement(&glNumberofServerLocks);
	}

	return S_OK;
}

STDAPI DllGetClassObject(const CLSID &clsid , REFIID riid , void**ppv)
{
	HRESULT hr;
	ClassFactory* pClassFactory = NULL;

	if (clsid != CLISD_CSquare)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}
	
	pClassFactory = new ClassFactory;

	hr = pClassFactory->QueryInterface(riid, ppv);
	
	if (FAILED(hr))
	{
		return E_NOINTERFACE;
	}

	return S_OK;
}

STDAPI DllCanUnloadNow(void)
{
	if ((glNumberOfActiveComponents == 0) && (glNumberofServerLocks == 0))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

STDAPI DllRegisterServer(void)
{
	return RegisterServer(L"AggregationOuterServer", ghModule, CLISD_CSquare);
}

STDAPI DllUnregisterServer(void)
{
	return UnregisterServer(CLISD_CSquare);
}


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		ghModule = (HMODULE)hInstance;
	}
	
	return TRUE;
}
















