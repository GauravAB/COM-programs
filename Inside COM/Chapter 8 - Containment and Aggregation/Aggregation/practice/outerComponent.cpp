#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include"interfaces.h"

void trace(const char* msg) { std::cout << msg << std::endl; }

//global variables

static HMODULE g_hModule = NULL;	//Dll Module 
static long g_cComponents = 0;		//Count of active components
static long g_cServerLocks = 0;     //Count of locks

//component code

class CA : public ISum
{
public:
	//IUnknown	
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//Interface ISum
	virtual int __stdcall SumOfTwoIntegers(int n1 , int n2) { return n1+n2; }
	
	HRESULT __stdcall initInnerComponent(void );

	//Constructor
	CA():m_cRef(1),m_pISub(NULL),m_pUnknownInner(NULL)
	{	
		std::cout<<"outerComponent\t m_cRef: "<<m_cRef<<std::endl;
		InterlockedIncrement(&g_cComponents);
	}
	//Destructor
	~CA()
	{
		InterlockedDecrement(&g_cComponents);
		trace("outerComponent: \t Destroy self.");

		//Prevent recursive destruction on next addref or release pair
		m_cRef = 1;
		IUnknown *pUnknownOuter =this;
		pUnknownOuter->AddRef();

		if(m_pISub != NULL)
		{
			m_pISub->Release();
		}

		if(m_pUnknownInner != NULL)
		{
			m_pUnknownInner->Release();
		}


	}

private:
	//Reference count
	long m_cRef;
	
	//Pointer to aggregated component's ISubtract Interface 
	ISubtract *m_pISub;

	//Pointer to inner components IUnknown
	IUnknown *m_pUnknownInner;

};


HRESULT __stdcall CA :: initInnerComponent()
{
	IUnknown *pUnknownOuter = this;
	trace("create inner component");

	HRESULT hr = CoCreateInstance(CLSID_CMathMore,pUnknownOuter,CLSCTX_INPROC_SERVER,IID_IUnknown,(void**)&m_pUnknownInner);

	if(FAILED(hr))
	{
		trace("could not create inner component");
		return E_FAIL;
	}

	trace("get ISubtract from inner component");
	hr = m_pUnknownInner->QueryInterface(IID_ISubtract,(void**)&m_pISub);

	if(FAILED(hr))
	{
		trace("Inner component does not support interface ISubtract");
		m_pUnknownInner->Release();
		m_pUnknownInner = NULL;

		m_pISub = NULL;
		return E_FAIL;
	}

	pUnknownOuter->Release();
	return S_OK;
}

HRESULT __stdcall CA::QueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<ISum*>(this);
	}
	else if (iid == IID_ISum)
	{
		*ppv = static_cast<ISum*>(this);
		trace("Component:\t\t Return pointer to ISum.");
	}
	else if (iid == IID_ISubtract)
	{
		m_pUnknownInner->QueryInterface(iid,ppv);
		trace("Component:\t\t Passing to inner components QueryInterface.");
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return S_OK;
}

ULONG __stdcall CA::AddRef(void)
{
	std::cout<<"outerComponent\t m_cRef: "<<InterlockedIncrement(&m_cRef)<<std::endl;
	return(m_cRef);

}

ULONG __stdcall CA::Release(void)
{
	long temp;

	temp = InterlockedDecrement(&m_cRef);
	std::cout<<"outerComponent\t m_cRef: "<<m_cRef<<std::endl;
	if (temp == 0)
	{
		delete(this);
		return 0;
	}
	return m_cRef;
}



class CFactory : public IClassFactory
{
public:
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void**ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	//interface IClassFactory
	virtual HRESULT __stdcall CreateInstance(IUnknown *pUnknownOuter, const IID& iid, void** ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);

	//constructor
	CFactory() 
	{
		m_cRef = 1;
	}
	//destructor
	~CFactory()
	{
		trace("Class Factory: \t\t Destroy Self.");
	}

private:
	long m_cRef;
};

//Class Factory IUnknown Implementation

HRESULT __stdcall CFactory::QueryInterface(const IID &iid, void** ppv)
{
	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
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

ULONG __stdcall CFactory::AddRef(void)
{
	return(InterlockedIncrement(&m_cRef));
}

ULONG __stdcall CFactory::Release(void)
{
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete (this);
		return 0;
	}

	return m_cRef;
}

//IClassFactory implementation

HRESULT __stdcall CFactory::CreateInstance(IUnknown* pUnknownOuter, const IID& iid, void **ppv)
{
	trace("Create instance : \t\t Create Component.");

	//cannot aggregate
	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	//create component

	CA *pA = new CA;
	if (pA == NULL)
	{
		return E_OUTOFMEMORY;
	}

	HRESULT hr = pA->initInnerComponent();
	if(FAILED(hr))
	{
		trace("failed to initialize inner component");
		pA->Release();
		return hr;
	}

	//Get the requested interface
	hr = pA->QueryInterface(iid, ppv);

	//Release the IUnknown Pointer
	// if query interface failed then the component will delete itself
	pA->Release();
	return hr;

}

HRESULT __stdcall CFactory::LockServer(BOOL bLock)
{
	if (bLock)
	{
		InterlockedIncrement(&g_cServerLocks);
	}
	else
	{
		InterlockedDecrement(&g_cServerLocks);
	}

	return S_OK;
}


STDAPI DllCanUnloadNow()
{
	if ((g_cComponents == 0) && (g_cServerLocks == 0))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

//Get Class Factory

STDAPI DllGetClassObject(const CLSID &clsid, const IID &iid, void **ppv)
{
	trace("DllGetClassObject:\t Create Class Factory");

	//Can we create this component
	if (clsid != CLSID_CMath)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	//Create class factory

	CFactory *pFactory = new CFactory;	//Reference count set to 1

	if (pFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	//Get requested interface
	HRESULT hr = pFactory->QueryInterface(iid, ppv);
	pFactory->Release();

	return hr;
}

//Dll module information

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void* lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hModule;
	}

	return TRUE;
}







