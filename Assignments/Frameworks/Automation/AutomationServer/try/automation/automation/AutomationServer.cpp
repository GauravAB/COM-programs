#include <Windows.h>
#include <stdio.h>
#include "AutomationServer.h"
#include "registry.h"

const char szFriendlyName[] = "AutomationServer";
const char szVerIndProgId[] = "AutomationServerWithoutRegFile";
const char szProgId[] = "AutomationServerWithoutRegFile_1_0";




//coclass declaration
class CMyMath : public IMyMath
{
private:
	long m_cRef;
	ITypeInfo *m_pITypeInfo = NULL;
public:
	//constructor method declaration
	CMyMath(void);
	//destructor method declaration
	~CMyMath(void);

	//IUnknown specific method declarations
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	//IDispatch specific method declarations (inherited)
	HRESULT __stdcall GetTypeInfoCount(UINT*);
	HRESULT __stdcall GetTypeInfo(UINT, LCID, ITypeInfo**);
	HRESULT __stdcall GetIDsOfNames(REFIID riid, LPOLESTR * rgszNames, UINT cNames, LCID lcid, DISPID *);
	HRESULT __stdcall Invoke(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*);

	//ISum Specific method declarations(inherited)

	HRESULT __stdcall SumOfTwoIntegers(int, int, int*);
	HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*);

	//custom methods
	HRESULT InitInstance(void);
};

//class factory declaration

class CMyMathClassFactory :  public IClassFactory
{
private:
	long m_cRef;
public:
	//constructor method declaration
	CMyMathClassFactory(void);
	~CMyMathClassFactory(void);

	//IUnknown specific method declarations
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	//IClassFactory inherited methods
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

//global Dll handle
HMODULE ghModule = NULL;

//global variable declaration
long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;


// {26E2F448-A7BF-4AC1-9FAF-15BAA779F2B5}
const GUID LIBID_AutomationServer = {
	0x26e2f448, 0xa7bf, 0x4ac1, 0x9f, 0xaf, 0x15, 0xba, 0xa7, 0x79, 0xf2, 0xb5
};

//Dll main

BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		ghModule = hDll;
		break;
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}

	return(TRUE);
}

 //Implementation of CMyMath's Constructor method

CMyMath::CMyMath(void)
{
	m_cRef = 1;		//hardcoded initialization of anticipate failure of the queryInterface

	InterlockedIncrement(&glNumberOfActiveComponents);
}


//Implementation of CMyMaths Destructor method
CMyMath :: ~CMyMath(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

//Implementation of CMyMath's IUnknown's Methods

HRESULT __stdcall CMyMath :: QueryInterface(REFIID riid, void **ppv)
{
	if(riid == IID_IUnknown)
	{
		*ppv = static_cast<IMyMath*>(this);
	}
	else if (riid == IID_IDispatch)
	{
		*ppv = static_cast<IMyMath*>(this);
	}
	else if (riid == IID_IMyMath)
	{
		*ppv = static_cast<IMyMath*>(this);
	}
	else
	{
		*ppv = NULL;
		return (E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}


ULONG __stdcall CMyMath::AddRef(void)
{
	return (InterlockedIncrement(&m_cRef));
}

ULONG __stdcall CMyMath::Release(void)
{
	InterlockedDecrement(&m_cRef);	

	if (m_cRef == 0)
	{
		m_pITypeInfo->Release();
		m_pITypeInfo = NULL;
		delete this;
		return 0;
	}

	return m_cRef;
}


HRESULT __stdcall CMyMath::SumOfTwoIntegers(int n1, int n2, int *n3)
{
	*n3 = n1 + n2;
	return S_OK;
}


HRESULT _stdcall CMyMath::SubtractionOfTwoIntegers(int n1, int n2, int *n3)
{
	*n3 = n1 - n2;
	return S_OK;
}


HRESULT CMyMath::InitInstance(void)
{
	//function declarations
	void ComErrorDescriptionString(HWND, HRESULT);

	//variable declarations
	HRESULT hr;
	ITypeLib *pITypeLib = NULL;

	//code 

	if (m_pITypeInfo = NULL)
	{
		hr = LoadRegTypeLib(LIBID_AutomationServer,
			1,	//major number
			0,	//minor number
			0x00,//LANG_NEUTRAL
			&pITypeLib);

		if (FAILED(hr))
		{
			ComErrorDescriptionString(NULL, hr);
			return (hr);
		}

		hr = pITypeLib->GetTypeInfoOfGuid(IID_IMyMath, &m_pITypeInfo);

		if (FAILED(hr))
		{

			ComErrorDescriptionString(NULL, hr);
			pITypeLib->Release();

			return hr;
		}

		pITypeLib->Release();
	}

	return S_OK;
}


CMyMathClassFactory::CMyMathClassFactory()
{
	m_cRef = 1;
}

CMyMathClassFactory::~CMyMathClassFactory()
{
	//nothing yet
}

HRESULT __stdcall CMyMathClassFactory::QueryInterface(REFIID iid, void **ppv)
{
	if (iid == IID_IUnknown || iid == IID_IClassFactory)
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG __stdcall CMyMathClassFactory::AddRef(void)
{
	return (InterlockedIncrement(&m_cRef));
}

ULONG __stdcall CMyMathClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}

	return m_cRef;
}


//IClassfactory methods 

HRESULT __stdcall CMyMathClassFactory :: CreateInstance(IUnknown* pUnknownOuter, REFIID riid, void**ppv)
{
	MessageBox(NULL, TEXT("CreateInstance called"), TEXT("query"), MB_OK);
	CMyMath *pCMyMath = NULL;
	HRESULT hr;

	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}
	
	pCMyMath = new CMyMath;
	
	if (pCMyMath == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCMyMath->QueryInterface(riid, ppv);
	
	if (FAILED(hr))
	{
		return E_NOINTERFACE;
	}

	pCMyMath->Release();

	return hr;
}

HRESULT __stdcall CMyMathClassFactory :: LockServer(BOOL bLock)
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

//Implementation of IDispatch methods
HRESULT CMyMath::GetTypeInfoCount(UINT *pCountTypeInfo)
{
	//as we have type library it is 1 or else it is 0
	*pCountTypeInfo = 1;
	return S_OK;
}

HRESULT CMyMath::GetTypeInfo(UINT iTypeInfo, LCID lcid, ITypeInfo **ppITypeInfo)
{
	*ppITypeInfo = NULL;

	if (iTypeInfo != 0)
	{
		return DISP_E_BADINDEX;
	}

	m_pITypeInfo->AddRef();
	*ppITypeInfo = m_pITypeInfo;
	return S_OK;
}


HRESULT CMyMath::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
	return (DispGetIDsOfNames(m_pITypeInfo, rgszNames, cNames, rgDispId));
}

HRESULT CMyMath::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT*puArgErr)
{
	//variable declarations
	HRESULT hr;
	hr = DispInvoke(this, m_pITypeInfo, dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	return hr;
}


//implementation of exported functions from this DLL

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	MessageBox(NULL, TEXT("DllGetClassObject"), TEXT("query"), MB_OK);
	//variable declarations
	CMyMathClassFactory *pCMyMathClassFactory = NULL;
	HRESULT hr;

	if (rclsid != CLSID_MyMath)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	//create class factory
	pCMyMathClassFactory = new CMyMathClassFactory;

	if (pCMyMathClassFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}
	 
	hr = pCMyMathClassFactory->QueryInterface(riid, ppv);

	pCMyMathClassFactory->Release(); //anticipate possible failure of QueryInterface
	return hr;
}

STDAPI DllCanUnloadNow(void)
{
	if((glNumberOfActiveComponents ==0) && (glNumberOfServerLocks == 0))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}


void ComErrorDescriptionString(HWND hwnd, HRESULT hr)
{
	TCHAR *szErrorMessage = NULL;
	TCHAR str[255];

	if (FACILITY_WINDOWS == HRESULT_FACILITY(hr))
	{
		hr = HRESULT_CODE(hr);
		if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&szErrorMessage, 0, NULL) != 0)
		{
			swprintf_s(str, TEXT("%#x: %s "), hr, szErrorMessage);
			LocalFree(szErrorMessage);
		}
		else
		{
			swprintf_s(str, TEXT("[Could not find a description for error # %#x.]\n"), hr);
		}

		MessageBox(NULL, str, TEXT("COM error"), MB_OK);
	}
}

STDAPI DllRegisterServer(void)
{
	return RegisterServer(ghModule, CLSID_MyMath, szFriendlyName, szVerIndProgId, szProgId);
}

STDAPI DllUnregisterServer(void)
{
	return UnregisterServer(CLSID_MyMath, szVerIndProgId, szProgId);
}


/*
STDAPI DllRegisterServer()
{
	HKEY hCLSIDKey = NULL, hInProcSvrKey = NULL;
	LONG lRet;
	TCHAR szModulePath[MAX_PATH];
	TCHAR szClassDescription[] = TEXT("Automation COM Class");
	TCHAR szThreadModel[] = TEXT("Apartment");

	__try
	{
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT, TEXT("CLSID\\{C5821FAB-506B-462E-A1B2-4C47138263E9}"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, NULL, &hCLSIDKey, NULL);
		
		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}


		//the default value of the key is a human readable description of co class
		lRet = RegSetValueEx(hCLSIDKey, NULL, 0, REG_SZ, (const BYTE*)szClassDescription, sizeof(szClassDescription));

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}
		
		//create the inprocserver32 key which holds info about our class
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT, TEXT("InProcServer32"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, NULL, &hInProcSvrKey, NULL);

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}

		GetModuleFileName(ghModule, szModulePath, MAX_PATH);

		lRet = RegSetValueEx(hInProcSvrKey, NULL, 0, REG_SZ, (const BYTE*)szModulePath, sizeof(szModulePath)/sizeof(char));
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

		if (NULL != hInProcSvrKey)
		{
			RegCloseKey(hInProcSvrKey);
		}
	}

	return S_OK;
}


STDAPI DllUnregisterServer()
{
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{C5821FAB-506B-462E-A1B2-4C47138263E9}\\InProcServer32"));

	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{C5821FAB-506B-462E-A1B2-4C47138263E9}"));

	return S_OK;
}
*/