#include <Windows.h>
#include "Header.h"

//Class declarations

class CSumSubtract : public ISum, ISubtract
{
private:
	long m_cref;
	//private reference counter
public:
	//constructor
	CSumSubtract(void);
	//destructor
	~CSumSubtract(void);
	//IUnknown Methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	//Isum methods
	HRESULT __stdcall SumOfTwoIntegers(int, int, int*);
	//Isubtract methods
	HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*);
};

class CSumSubtractClassFactory : public IClassFactory
{
private:
	long m_cref;
public:
		CSumSubtractClassFactory(void);
	~CSumSubtractClassFactory(void);
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	//IClass factory specific methods
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

//global variable declarations

long glNumberOfActiveComponents = 0;
long glNumberOfServiceLocks = 0;

BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}
	return(TRUE);
}

CSumSubtract::CSumSubtract(void)
{
	m_cref = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CSumSubtract :: ~CSumSubtract(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT CSumSubtract::QueryInterface(REFIID riid, void **ppv)
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
		return(E_NOINTERFACE);
	}
		
		reinterpret_cast <IUnknown*>(*ppv)->AddRef();
		return (S_OK);
}

ULONG CSumSubtract::AddRef(void)
{
	InterlockedIncrement(&m_cref);
	return(m_cref);
}

ULONG CSumSubtract::Release(void)
{
	InterlockedDecrement(&m_cref);
	if (m_cref == 0)
	{
		delete(this);
		return(0);
	}
	return(m_cref);
}

HRESULT CSumSubtract::SumOfTwoIntegers(int num1, int num2, int * res)
{
	*res = num1 + num2;

	return(S_OK);

}

HRESULT CSumSubtract::SubtractionOfTwoIntegers(int num1, int num2, int *res)
{
	*res = num1 - num2;

	return(S_OK);
}

CSumSubtractClassFactory :: CSumSubtractClassFactory(void)
{
	m_cref = 1;
}

CSumSubtractClassFactory:: ~CSumSubtractClassFactory(void)
{
	//nothing 
}

HRESULT CSumSubtractClassFactory::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else if (riid == IID_IClassFactory)
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return(S_OK);
}

ULONG CSumSubtractClassFactory::AddRef(void)
{
	InterlockedIncrement(&m_cref);
	
	return(m_cref);
}


ULONG CSumSubtractClassFactory::Release(void)
{
	InterlockedDecrement(&m_cref);

	if (m_cref == 0)
	{
		delete(this);
		return(0);
	}
	return(m_cref);
}

HRESULT CSumSubtractClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
	CSumSubtract *pCSumSubtract = NULL;
	HRESULT hr;

	if (pUnkOuter != NULL)
	{
		return(CLASS_E_NOAGGREGATION);
	}
	pCSumSubtract = new CSumSubtract;

	if (pCSumSubtract == NULL)
	{
		return(E_OUTOFMEMORY);
	}
	hr = pCSumSubtract->QueryInterface(riid, ppv);
	pCSumSubtract->Release();
	return(hr);

}



HRESULT CSumSubtractClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
	{
		InterlockedIncrement(&glNumberOfActiveComponents);

	}
	else
	{
		InterlockedDecrement(&glNumberOfActiveComponents);
	}

	return(S_OK);
}


HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	CSumSubtractClassFactory *pCSumSubtractClassFactory = NULL;
	HRESULT hr;

	if (rclsid != CLSID_SumSubtract)
	{
		return(CLASS_E_CLASSNOTAVAILABLE);
	}
	pCSumSubtractClassFactory = new CSumSubtractClassFactory;

	if (pCSumSubtractClassFactory == NULL)
	{
		return(E_OUTOFMEMORY);
	}
	
	hr = pCSumSubtractClassFactory->QueryInterface(riid, ppv);

	if (hr != S_OK)
	{
		//MessageBox(NULL, TEXT("Classfactory QueryInterface failed"),0);
		exit(0);
	}
	pCSumSubtractClassFactory->Release();
	return(hr);
}


HRESULT __stdcall DllCanUnloadNow(void)
{
	if ((glNumberOfActiveComponents == 0) && (glNumberOfServiceLocks == 0))
	{
		return(S_OK);
	}
	else
	{
		return(S_FALSE);
	}
}


 









































