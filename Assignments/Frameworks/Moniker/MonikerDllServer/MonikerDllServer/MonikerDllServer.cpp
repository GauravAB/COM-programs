#include <Windows.h>
#include "MonikerDllServer.h"
#include "register.h"


class COddNumber : public IOddNumber
{
private:
	long m_cRef;
	int m_iFirstOddNumber;
public:
	COddNumber(int);
	~COddNumber(void);

	//IUnknown specific method declarations
	HRESULT __stdcall QueryInterface(REFIID riid, void** ppv);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	//This class methods
	BOOL __stdcall IsOdd(int);

	//IOddNumber specific method declarations (inherited)
	HRESULT __stdcall GetNextOddNumber(int*);
};

class COddNumberFactory : public IOddNumberFactory
{
private:
	long m_cRef;
public:
	COddNumberFactory(void);
	~COddNumberFactory(void);

	//IUnknown
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	//OddNumberFactory specific methods
	HRESULT __stdcall SetFirstOddNumber(int, IOddNumber**);	
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

COddNumber::COddNumber(int iFirstOddNumber)
{
	//hardcoded initialization to anticipate possible failure of queryinterface
	m_cRef = 1;
	m_iFirstOddNumber = iFirstOddNumber;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

COddNumber::~COddNumber(void)
{
	InterlockedDecrement(&m_cRef);
}

HRESULT __stdcall COddNumber::QueryInterface(REFIID riid, void**ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<IOddNumber*>(this);
	}
	else if(riid == IID_IOddNumber)
	{
		*ppv = static_cast<IOddNumber*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	
	return S_OK;
}



ULONG COddNumber::AddRef(void)
{
	return InterlockedIncrement(&m_cRef);
}


ULONG COddNumber::Release(void)
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}

	return m_cRef;
} 

BOOL COddNumber::IsOdd(int iFirstOddNumber)
{
	if (iFirstOddNumber != 0 && iFirstOddNumber % 2 != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


HRESULT COddNumber::GetNextOddNumber(int *pNextOddNumber)
{
	//variable declarations
	BOOL bResult;
	bResult = IsOdd(m_iFirstOddNumber + 2);
	
	if (bResult == TRUE)
	{
		*pNextOddNumber = m_iFirstOddNumber + 2;
	}
	else
	{
		return S_FALSE;
	}

	return S_OK;
}



COddNumberFactory::COddNumberFactory(void)
{

	m_cRef = 1;
}

COddNumberFactory:: ~COddNumberFactory(void)
{
	//code 
}

HRESULT __stdcall COddNumberFactory::QueryInterface(REFIID riid, void**ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<IOddNumberFactory*>(this);
	
	}
	else if (riid == IID_IOddNumberFactory)
	{
		*ppv = static_cast<IOddNumberFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}


ULONG COddNumberFactory::AddRef(void)
{
	InterlockedIncrement(&m_cRef);

	return (m_cRef);
}



ULONG COddNumberFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete this;

		return 0;
	}

	return m_cRef;
}



HRESULT COddNumberFactory::SetFirstOddNumber(int iFirstOddNumber, IOddNumber **ppIOddNumber)
{
	HRESULT hr;
	COddNumber* pCOddNumber = new COddNumber(iFirstOddNumber);

	if (pCOddNumber == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCOddNumber->QueryInterface(IID_IOddNumber, (void**) ppIOddNumber);
	pCOddNumber->Release();

	return (hr);
}

//Implementation of exported functions from this DLL

HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid , void**ppv)
{
	COddNumberFactory *pCOddNumberFactory = NULL;
	HRESULT hr;

	if (rclsid != CLSID_OddNumber)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	pCOddNumberFactory = new COddNumberFactory;

	if (pCOddNumberFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCOddNumberFactory->QueryInterface(riid, ppv);
	pCOddNumberFactory->Release();

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
	return RegisterServer(L"simpleMonikerClass",ghModule,CLSID_OddNumber);
}

STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_OddNumber);
}












