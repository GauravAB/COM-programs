#include <Windows.h>
#include <stdio.h>
#include "AggregationInnerServer.h"
#include "register.h"
#include "Util.h"
#include <cmath>

interface INondelegatingUnknown
{
	virtual HRESULT __stdcall NondelegatingQueryInterface(REFIID riid, void**ppv)=0;
	virtual ULONG  __stdcall NondelegatingAddRef(void) = 0;
	virtual ULONG __stdcall NondelegatingRelease(void) = 0;
};

//globals
long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;
HMODULE ghModule;


class CSquareroot : public ISquareroot , public INondelegatingUnknown
{
private:
	long m_cRef;
	IUnknown* m_pUnknownOuter;
public:
	//IUnknown Methods
	virtual HRESULT __stdcall QueryInterface(REFIID, void**);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//class methods
	CSquareroot(IUnknown* pUnknownOuter);
	~CSquareroot();

	//Interface methods
	virtual HRESULT __stdcall SquarerootOfAnInteger(int, int*);
	//INondelegating methods
	virtual HRESULT __stdcall NondelegatingQueryInterface(REFIID, void**);
	virtual ULONG __stdcall NondelegatingAddRef(void);
	virtual ULONG __stdcall NondelegatingRelease(void);
};

class ClassFactory : public IClassFactory
{
public:
	//IUnknown methods
	virtual HRESULT __stdcall QueryInterface(REFIID, void**);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);
	
	//Class specific
	ClassFactory(void);
	~ClassFactory(void);

	//IClassFactory
	virtual HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	virtual HRESULT __stdcall LockServer(BOOL);
private:
	long m_cRef;
};


//CoClass methods
CSquareroot::CSquareroot(IUnknown* pUnknownOuter)
{
	if (pUnknownOuter == NULL)
	{
		m_pUnknownOuter =  reinterpret_cast<IUnknown*>(static_cast<INondelegatingUnknown*>(this));
	}
	else
	{
		m_pUnknownOuter = pUnknownOuter;
	}

	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CSquareroot::~CSquareroot()
{
	if (m_pUnknownOuter)
	{
		m_pUnknownOuter->Release();
		m_pUnknownOuter = NULL;
	}

	InterlockedDecrement(&glNumberOfActiveComponents);
}


HRESULT __stdcall CSquareroot::QueryInterface(REFIID riid, void**ppv)
{
	return m_pUnknownOuter->QueryInterface(riid, ppv);
}

ULONG __stdcall CSquareroot::AddRef(void)
{
	return m_pUnknownOuter->AddRef();
}

ULONG __stdcall CSquareroot::Release(void)
{
	return m_pUnknownOuter->Release();
}

HRESULT __stdcall CSquareroot::SquarerootOfAnInteger(int num, int* res)
{

	*res = sqrt(num);
	return S_OK;
}


HRESULT __stdcall CSquareroot::NondelegatingQueryInterface(REFIID riid, void**ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<INondelegatingUnknown*>(this);
	}
	else if (riid == IID_ISquareroot)
	{
		*ppv = static_cast<ISquareroot*>(this);
	}
	else
	{
		*ppv = NULL;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return S_OK;
}

ULONG __stdcall CSquareroot::NondelegatingAddRef(void)
{
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CSquareroot::NondelegatingRelease(void)
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete this;
	}
	return m_cRef;
}


//ClassFactory methods
ClassFactory::ClassFactory()
{
	m_cRef = 1;
}

ClassFactory::~ClassFactory()
{
	//nothing for now
}

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
	return S_OK;
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

HRESULT __stdcall ClassFactory::CreateInstance(IUnknown* pUnknownOuter, REFIID riid, void**ppv)
{
	HRESULT hr;
	CSquareroot* pSquareroot = NULL;

	if ((pUnknownOuter != NULL) && (riid != IID_IUnknown))
	{
		return CLASS_E_NOAGGREGATION;
	}

	pSquareroot = new CSquareroot(pUnknownOuter);
	
	if (pSquareroot == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pSquareroot->NondelegatingQueryInterface(riid, ppv);
	if (FAILED(hr))
	{
		return E_NOINTERFACE;
	}

	pSquareroot->NondelegatingRelease();
	
	return hr;
}

HRESULT __stdcall ClassFactory::LockServer(BOOL bLock)
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


STDAPI DllCanUnloadNow()
{
	if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

STDAPI DllGetClassObject(const CLSID &clsid , REFIID riid , void**ppv)
{
	HRESULT hr;
	ClassFactory* pClassFactory = NULL;
	if (clsid != CLSID_CSquareroot)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	pClassFactory = new ClassFactory;
	
	hr = pClassFactory->QueryInterface(riid, ppv);
	if (FAILED(hr))
	{
		return E_NOINTERFACE;
	}

	pClassFactory->Release();	
	return hr;
}

STDAPI DllRegisterServer()
{
	return RegisterServer(L"AggregationInnerServer", ghModule, CLSID_CSquareroot);
}
STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_CSquareroot);
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		ghModule = (HMODULE)hInstance;
	}

	return TRUE;
}




