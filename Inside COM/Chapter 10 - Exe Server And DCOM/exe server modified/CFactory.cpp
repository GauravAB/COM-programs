#include <windows.h>
#include "CFactory.h"
#include "Interface.h"
#include "Component1.h"
#include "Server.h"


void trace(const char*msg)
{
	Util::Trace("CFactory:", msg,S_OK);
}

void trace(const char*msg,HRESULT hr)
{
	Util::Trace("CFactory:", msg,hr);
}


LONG CFactory::s_cServerLocks = 0;
DWORD CFactory::s_dwThreadID = 0;



CFactory::CFactory() :m_cRef(1) {}
CFactory::~CFactory() {}

HRESULT __stdcall CFactory::QueryInterface(const IID &iid, void **ppv)
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
	return S_OK;
}

ULONG __stdcall CFactory::AddRef()
{
	::InterlockedIncrement(&m_cRef);
	return m_cRef;
}

ULONG __stdcall CFactory::Release()
{
	::InterlockedDecrement(&m_cRef);
	
	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

//IClassfactory implementation
HRESULT __stdcall CFactory::CreateInstance(IUnknown *pUnknownOuter, const IID &iid, void**ppv)
{
	//Aggregation only if requested IID is IID_IUNKNOWN
	if ( (pUnknownOuter != NULL) && (iid != IID_IUnknown))
	{
		return CLASS_E_NOAGGREGATION;
	}

	//create the component
	CSumSubtract* pCSumSubtract = new CSumSubtract(NULL);

	
	HRESULT hr = pCSumSubtract->InitializeInnerComponent();
	if (FAILED(hr))
	{
		pCSumSubtract->NondelegatingRelease();
		return hr;
	}

	hr = pCSumSubtract->NondelegatingQueryInterface(iid, ppv);
	if (FAILED(hr))
	{
		trace("failed to get the required interface");
	}

	pCSumSubtract->NondelegatingRelease();

	return hr;
}


HRESULT __stdcall CFactory::LockServer(BOOL bLock)
{
	if (bLock)
	{
		::InterlockedIncrement(&s_cServerLocks);
	}
	else
	{
		::InterlockedDecrement(&s_cServerLocks);
	}

	return S_OK;
}


HRESULT CFactory::StartFactories()
{
	HRESULT hr;

	//get the class factory which creates our component object
	gpIClassFactory = new CFactory;
	if (gpIClassFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	//Register our class factory in COMS database
	hr = CoRegisterClassObject(CLSID_CSumSubtract, static_cast<IUnknown*>(gpIClassFactory), CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &dwRegister);

	if (FAILED(hr))
	{
		gpIClassFactory->Release();
		return(E_FAIL);
	}
	return S_OK;
}

void CFactory::StopFactories(void)
{
	if (dwRegister != NULL)
	{
		CoRevokeClassObject(dwRegister);
	}
	if (gpIClassFactory != NULL)
	{
		gpIClassFactory->Release();
	}

	//return S_OK;
}















