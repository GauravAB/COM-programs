#include <Windows.h>
#include "MonikerDllServer.h"
#include "register.h"
#include <cstdlib>
#include "Util.h"


void trace(const char *msg)
{
	Util::Trace("Moniker Server ", msg, S_OK);
}

class CRandomNumber : public IRandomNumber
{
private:
	long m_cRef;
	int m_iSeed;
public:
	CRandomNumber(int);
	~CRandomNumber(void);

	//IUnknown specific method declarations
	HRESULT __stdcall QueryInterface(REFIID riid, void** ppv);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	//This class methods
	BOOL __stdcall IsValid(int);
	HRESULT __stdcall changeSeed(int);

	//IOddNumber specific method declarations (inherited)
	HRESULT __stdcall GetNextRandomNumber(int*);
};

class CRandomNumberFactory : public IRandomNumberFactory
{
private:
	long m_cRef;
public:
	CRandomNumberFactory(void);
	~CRandomNumberFactory(void);

	//IUnknown
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	//OddNumberFactory specific methods
	HRESULT __stdcall SetSeed(int, IRandomNumber**);
};


//global variables
long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;
HMODULE ghModule = NULL;


BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
			ghModule = (HMODULE)hDll;
			break;
		case DLL_PROCESS_DETACH:
			break;
		default:
			break;
	}
	return TRUE;
}

//Implementations

CRandomNumber::CRandomNumber(int iSeed)
{
	
	//hardcoded initialization to anticipate possible failure of queryinterface
	m_cRef = 1;
	m_iSeed = iSeed;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CRandomNumber::~CRandomNumber(void)
{
	InterlockedDecrement(&m_cRef);
}

HRESULT __stdcall CRandomNumber::QueryInterface(REFIID riid, void**ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<IRandomNumber*>(this);
	}
	else if(riid == IID_IRandomNumber)
	{
		*ppv = static_cast<IRandomNumber*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	
	return S_OK;
}



ULONG CRandomNumber::AddRef(void)
{
	return InterlockedIncrement(&m_cRef);
}


ULONG CRandomNumber::Release(void)
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
} 

BOOL CRandomNumber::IsValid(int iFirstRandomNumber)
{
	if (iFirstRandomNumber < 11)
	{
		
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


HRESULT CRandomNumber::changeSeed(int iSeed)
{
	if (iSeed < 11)
	{
		m_iSeed = iSeed;
	}
	else
	{
		m_iSeed = iSeed % 11;
	}

	return S_OK;
}

HRESULT CRandomNumber::GetNextRandomNumber(int *pNextRandomNumber)
{
	//variable declarations
	BOOL bResult;
	bResult = IsValid (m_iSeed);
	
	if (bResult == TRUE)
	{
		trace("setting next number");
		srand(m_iSeed);
		*pNextRandomNumber = rand() % 11;
	}
	else
	{
		return S_FALSE;
	}

	return S_OK;
}

CRandomNumberFactory::CRandomNumberFactory(void)
{

	m_cRef = 1;
}

CRandomNumberFactory:: ~CRandomNumberFactory(void)
{
	//code 
}

HRESULT __stdcall CRandomNumberFactory::QueryInterface(REFIID riid, void**ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<IRandomNumberFactory*>(this);
	
	}
	else if (riid == IID_IRandomNumberFactory)
	{
		*ppv = static_cast<IRandomNumberFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}


ULONG CRandomNumberFactory::AddRef(void)
{
	InterlockedIncrement(&m_cRef);
	return (m_cRef);
}



ULONG CRandomNumberFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}

	return m_cRef;
}

HRESULT CRandomNumberFactory::SetSeed(int iFirstRandomNumber, IRandomNumber **ppIRandomNumber)
{
	HRESULT hr;
	CRandomNumber* pCRandomNumber = new CRandomNumber(iFirstRandomNumber);

	if (pCRandomNumber == NULL)
	{
		return E_OUTOFMEMORY;
	}
	hr = pCRandomNumber->QueryInterface(IID_IRandomNumber, (void**) ppIRandomNumber);
	pCRandomNumber->Release();

	return (hr);
}

//Implementation of exported functions from this DLL
HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid , void**ppv)
{
	CRandomNumberFactory *pCRandomNumberFactory = NULL;
	HRESULT hr;

	if (rclsid != CLSID_RandomNumber)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	pCRandomNumberFactory = new CRandomNumberFactory;

	if (pCRandomNumberFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCRandomNumberFactory->QueryInterface(riid, ppv);
	pCRandomNumberFactory->Release();
	return hr;
}

HRESULT __stdcall DllCanUnloadNow()
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

STDAPI DllRegisterServer()
{
	return RegisterServer(L"simpleMonikerClass",ghModule,CLSID_RandomNumber);
}

STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_RandomNumber);
}












