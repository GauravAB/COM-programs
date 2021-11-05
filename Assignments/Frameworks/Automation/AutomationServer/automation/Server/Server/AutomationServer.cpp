#include <Windows.h>
#include <stdio.h>
#include "AutomationServer.h"
#include <cmath>
#include "Util.h"
#include "register.h"

void trace(const char*msg, HRESULT hr)
{
	Util::Trace("AutomationServer", msg, hr);
}

void trace(const char*msg)
{
	Util::Trace("AutomationServer", msg, S_OK);
}
   


//globals
long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;
HMODULE ghModule;


class CCubeCuberoot : public ICubeCuberoot
{
private:
	long m_cRef;
	ITypeInfo* m_pITypeInfo;

public:
	//Constructor methods declaration
	CCubeCuberoot(void);
	~CCubeCuberoot(void);

	//IUnknown methods	
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void**ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//Interface methods
	virtual HRESULT __stdcall CubeOfNumber(int,int*);
	virtual HRESULT __stdcall CubeRootOfNumber(int, int*);

	//IDispatch methods
	virtual HRESULT __stdcall GetTypeInfoCount(UINT *pCountTypeInfo);
	virtual HRESULT __stdcall GetTypeInfo(UINT, LCID ,ITypeInfo**);
	virtual HRESULT __stdcall GetIDsOfNames(REFIID, LPOLESTR*, UINT, LCID, DISPID*);
	virtual HRESULT __stdcall Invoke(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*);

	//custom methods
	HRESULT __stdcall InitInstance(void);
};


class ClassFactory : public IClassFactory
{
private:
	long m_cRef;

public:
	ClassFactory(void);
	~ClassFactory(void);

	//IUnknown specific method declarations(inherited)
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	//IClassFactory specific method declarations(inherited)
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

//DllMain

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		ghModule = (HMODULE)hInstance;
		break;
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}
	return TRUE;
}

//class method definations

CCubeCuberoot::CCubeCuberoot(void)
{
	m_cRef = 1;
	m_pITypeInfo = NULL;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CCubeCuberoot ::~CCubeCuberoot(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT __stdcall CCubeCuberoot::QueryInterface(REFIID riid, void** ppv)
{
	if ((riid == IID_IUnknown) || (riid == IID_ICubeCuberoot) || (riid == IID_IDispatch))
	{
		*ppv = static_cast<ICubeCuberoot*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return S_OK;
}

ULONG __stdcall CCubeCuberoot::AddRef(void)
{
	return InterlockedIncrement(&m_cRef);
}

ULONG _stdcall CCubeCuberoot::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}

	return m_cRef;
}

HRESULT __stdcall CCubeCuberoot::CubeOfNumber(int n1, int *res)
{
	*res = pow(n1,3);
	return S_OK;
}

HRESULT __stdcall CCubeCuberoot::CubeRootOfNumber(int n1, int*res)
{
	*res = cbrt(n1);

	return S_OK;
}

HRESULT __stdcall CCubeCuberoot::InitInstance (void)
{
	//function declaration
	HRESULT hr;
	ITypeLib* pITypeLib = NULL;

	//Code
	if (m_pITypeInfo == NULL)
	{
		hr = LoadRegTypeLib(LIBID_AutomationServer, 1, 0, 0x00, &pITypeLib);
		if (FAILED(hr))
		{
			trace("LoadRegTypeLib", hr);
			return hr;
		}

		hr = pITypeLib->GetTypeInfoOfGuid(IID_ICubeCuberoot, &m_pITypeInfo);
		if (FAILED(hr))
		{
			trace("LoadRegTypeLib", hr);
			pITypeLib->Release();
			return(hr);
		}
		pITypeLib->Release();
	}

	return S_OK;
}



//CLASSFACTORY DEFINATIONS 

ClassFactory::ClassFactory()
{
	m_cRef = 1;
}

ClassFactory::~ClassFactory()
{
	//nothing for us
}


HRESULT __stdcall ClassFactory::QueryInterface(REFIID riid, void** ppv)
{
	if ((riid == IID_IUnknown) || (riid == IID_IClassFactory))
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
}

ULONG __stdcall ClassFactory::AddRef(void)
{
	return InterlockedIncrement(&m_cRef);
}

ULONG _stdcall ClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}

	return m_cRef;
}


HRESULT __stdcall ClassFactory :: CreateInstance(IUnknown* pUnknownOuter, REFIID riid, void**ppv)
{
	HRESULT hr;
	CCubeCuberoot *pCCubeCuberoot = NULL;

	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}
	pCCubeCuberoot = new CCubeCuberoot;

	if(pCCubeCuberoot == NULL)
	{
		return E_OUTOFMEMORY;
	}
	
	//call automation related init method
	pCCubeCuberoot->InitInstance();

	hr = pCCubeCuberoot->QueryInterface(riid, ppv);

	if (FAILED(hr))
	{
		pCCubeCuberoot->Release();
		return E_NOINTERFACE;
	}

	pCCubeCuberoot->Release();

	return hr;
}

HRESULT __stdcall ClassFactory :: LockServer(BOOL bLock)
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


//IDispatch methods

HRESULT __stdcall CCubeCuberoot::GetTypeInfoCount(UINT *pCountTypeInfo)
{
	//as we do have library the count is 1 or else it would have been 0
	*pCountTypeInfo = 1;
	return S_OK;
}


HRESULT CCubeCuberoot::GetTypeInfo(UINT iTypeInfo, LCID lcid, ITypeInfo **ppITypeInfo)
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

HRESULT CCubeCuberoot::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid , DISPID *rgDispId)
{
	return (DispGetIDsOfNames(m_pITypeInfo, rgszNames, cNames, rgDispId));
}

HRESULT CCubeCuberoot::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	HRESULT hr;

	hr = DispInvoke(this, m_pITypeInfo, dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	return hr;
}


//Exported Functions
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void**ppv)
{
	//variable declaration
	ClassFactory *pClassFactory;
	HRESULT hr;

	if (rclsid != CLSID_CCubeCubeRoot)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	pClassFactory = new ClassFactory;
	if (pClassFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pClassFactory->QueryInterface(riid, ppv);
	if (FAILED(hr))
	{
		trace("DllGetClassObject", hr);
	}

	pClassFactory->Release();
	return hr;
}

STDAPI  DllCanUnloadNow(void)
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
	return RegisterServer(L"Automation COM Server", ghModule, CLSID_CCubeCubeRoot);
}

STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_CCubeCubeRoot);
}





