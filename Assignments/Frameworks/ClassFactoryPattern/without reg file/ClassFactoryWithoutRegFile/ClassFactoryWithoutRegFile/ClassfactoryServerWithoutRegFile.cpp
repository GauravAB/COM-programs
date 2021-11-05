#include <Windows.h>
#include <stdio.h>
#include "ClassFactoryServerWithoutRegFile.h"
#include "Util.h"
#include "register.h"

void trace(const char *msg)
{
	Util::Trace("CSumSubtractServer", msg, S_OK);
}

void trace(const char* msg, HRESULT hr)
{
	Util::Trace("CSumSubtractServer", msg, hr);
}

long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;
HMODULE ghModule;

class CSumSubtract : public ISum, public ISubtract
{
private:
	long m_cRef;
public:
	CSumSubtract(void);
	~CSumSubtract(void);

	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int*);
	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*);
};


class myClassFactory: public IClassFactory
{
private:
	long m_cRef;
public:
	
	myClassFactory();
	~myClassFactory();

	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//IClassFactorySpecific methods

	virtual HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	virtual HRESULT __stdcall LockServer(BOOL);
};

//defination CSumSubtract

CSumSubtract::CSumSubtract(void)
{
	//hardcoded increment 1 
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CSumSubtract:: ~CSumSubtract(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

//IUnknown Methods
HRESULT __stdcall CSumSubtract::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<ISum*>(this);
	}
	else if (riid == IID_ISum)
	{
		*ppv = static_cast<ISum*>(this);
	}
	else if (riid == IID_ISubtract)
	{

		*ppv = static_cast<ISubtract*>(this);
	}
	else
	{
		*ppv = NULL;
		return 0;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}


ULONG __stdcall CSumSubtract::AddRef(void)
{
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CSumSubtract::Release(void)
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}

	return m_cRef;
}


HRESULT CSumSubtract::SumOfTwoIntegers(int n1, int n2, int *n3)
{
	*n3 = n1 + n2;

	return S_OK;
}

HRESULT CSumSubtract::SubtractionOfTwoIntegers(int n1, int n2, int* n3)
{
	*n3 = n1 - n2;

	return S_OK;
}


//ClassFactory methods

myClassFactory::myClassFactory(void)
{
	m_cRef = 1;
}

myClassFactory::~myClassFactory(void)
{
	//nothing to do
}

HRESULT myClassFactory::QueryInterface(REFIID riid, void **ppv)
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

ULONG myClassFactory::AddRef(void)
{
	return (InterlockedIncrement(&m_cRef));
}

ULONG myClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}

	return m_cRef;
}

//IClassFactory methods

HRESULT myClassFactory::CreateInstance(IUnknown *pUnknownOuter, REFIID riid, void**ppv)
{
	trace("create Instance");
	CSumSubtract *pCSumSubtract = NULL;
	HRESULT hr;

	if(pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	pCSumSubtract = new CSumSubtract;

	if (pCSumSubtract == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCSumSubtract->QueryInterface(riid, ppv);

	if (FAILED(hr))
	{
		return E_NOINTERFACE;
	}
	pCSumSubtract->Release();
	return S_OK;
}

HRESULT myClassFactory::LockServer(BOOL bLock)
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

STDAPI DllGetClassObject(const CLSID &clsid, REFIID riid, void**ppv)
{
	trace("DllGetClassObject");
	myClassFactory *pmyClassFactory = NULL;
	HRESULT hr;

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


STDAPI DllCanUnloadNow(void)
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
	return RegisterServer(L"Classfactory COM Server",ghModule,CLSID_CSumSubtract);
}

STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_CSumSubtract);
}

/*
STDAPI DllRegisterServer(void)
{
	HKEY hCLSIDKey = NULL;
	HKEY hInProcServerKey = NULL;
	LONG lRet;
	TCHAR szModulePath[MAX_PATH];
	TCHAR szClassDescription[] = TEXT("ClassFactory COM class");
	TCHAR szThreadingModel[] = TEXT("Apartment");

	__try
	{
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT, TEXT("CLSID\\{07255CAA-D856-4167-B8B8-BC5A9A4A0CCB}"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, NULL, &hCLSIDKey, NULL);

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}
		
		lRet = RegSetValueEx(hCLSIDKey, NULL, 0, REG_SZ, (const BYTE*)szClassDescription, sizeof(szClassDescription));

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}

		//create the inprocserver32 key , which holds info about our coclass
		
		lRet = RegCreateKeyEx(hCLSIDKey, TEXT("InProcServer32"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hInProcServerKey, NULL);
		
		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}
		
		//The default value of the inprocServer32 holds full dll path

		GetModuleFileName(ghModule, szModulePath, MAX_PATH);

		lRet = RegSetValueEx(hInProcServerKey, NULL, 0, REG_SZ, (const BYTE*)szModulePath, sizeof(TCHAR)*(lstrlen(szModulePath) + 1));

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}
		
		//the threading model tells COM how to handle threads

		lRet = RegSetValueEx(hInProcServerKey, TEXT("ThreadingModel"), 0, REG_SZ, (const BYTE*)szThreadingModel, sizeof(szThreadingModel));

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}	
	}

	__finally
	{

		if (NULL != hCLSIDKey)
		{
			RegCloseKey(hCLSIDKey);
		}

		if (NULL != hInProcServerKey)
		{

			RegCloseKey(hInProcServerKey);
		}
	}

	return S_OK;
}

STDAPI DllUnregisterServer(void)
{
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{07255CAA-D856-4167-B8B8-BC5A9A4A0CCB}\\InProcServer32"));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{07255CAA-D856-4167-B8B8-BC5A9A4A0CCB}"));

	return S_OK;
}
*/

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		ghModule = (HMODULE)hModule;
	}
	return TRUE;
}












