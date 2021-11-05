#include "Registry.h"
#include "CFactory.h"


//static variables

LONG CFactory::s_cServerLocks = 0;		//count of locks
HMODULE CFactory::s_hModule = NULL;		//DLL module handle

#ifdef __OUTPROC_SERVER_
DWORD CFactory::s_dwThreadID = 0;
#endif


//CFactory implementation

CFactory::CFactory(const CFactoryData* pCFactoryData):m_cRef(1)
{
	m_pFactoryData = pCFactoryData;
}

//IUnknown implementation
HRESULT __stdcall CFactory::QueryInterface(REFIID iid, void**ppv)
{
	IUnknown* pI;

	if((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		pI = this;
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	pI->AddRef();
	*ppv = pI;

	return S_OK;
}


ULONG __stdcall CFactory ::AddRef()
{
	return (::InterlockedIncrement(&m_cRef));
}

ULONG __stdcall CFactory::Release()
{
	if(::InterlockedDecrement(&m_cRef) == 0)
	{
		delete this;
		return 0;
	}

	return m_cRef;
}

HRESULT __stdcall CFactory::CreateInstance(IUnknown* pUnknownOuter,const IID &iid , void **ppv)
{
	//Aggregate only if the required IID is IID_IUnknown
	if((pUnknownOuter != NULL) && (iid != IID_IUnknown))
	{
		return CLASS_E_NOAGGREGATION;
	}

	//Initialize the component
	CUnknown* pNewComponent;
	HRESULT hr = m_pFactoryData->CreateInstance(pUnknownOuter,&pNewComponent);

	if(FAILED(hr))
	{
		return hr;
	}

	//Initialize the component
	hr = pNewComponent->InitializeInnerComponent();
	if(FAILED(hr))
	{
		return E_FAIL;
	}
}
















