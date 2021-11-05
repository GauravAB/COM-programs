#include <Windows.h>
#include "CFactory.h"
#include "REGISTRY.H"
#include "UTIL.h"
#include "interface.h"

static void trace(const char* msg)
{
	Util::Trace("CFactory", msg, S_OK);
}

static void trace(const char* msg, HRESULT hr)
{
	Util::Trace("CFactory ", msg, hr);
}

LONG CFactory::s_cServerLocks = 0;	//count of locks

HMODULE CFactory::s_hModule = NULL;	//Dll module handle

//CFactory implementation

CFactory::CFactory(const CFactoryData* pFactoryData):m_cRef(1)
{
	m_pFactoryData = pFactoryData;
}

//IUnknown Implementation
HRESULT __stdcall CFactory::QueryInterface(REFIID iid, void **ppv)
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
	return (InterlockedIncrement(&m_cRef));
}


ULONG __stdcall CFactory::Release(void)
{
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete this;
		return 0;
	}

	return m_cRef;
}


//IClassFactory Implementation
HRESULT __stdcall CFactory::CreateInstance(IUnknown *pUnknownOuter, const IID &iid, void **ppv)
{
	//Aggregate only if the requested IID is IID_IUnknown

	if ((pUnknownOuter != NULL) && (iid != IID_IUnknown))
	{
		return CLASS_E_NOAGGREGATION;
	}

	//create the component
	CUnknown* pNewComponent;

	HRESULT hr = m_pFactoryData->CreateInstance(pUnknownOuter, &pNewComponent);
	if (FAILED(hr))
	{
		return hr;
	}

	//Initialize the component
	hr = pNewComponent->Init();

	if (FAILED(hr))
	{
		//Initialization failed. Release the component
		pNewComponent->NonDelegatingRelease();
		
		return (hr);
	}

	//get the requested interface
	hr = pNewComponent->NonDelegatingQueryInterface(iid, ppv);
	
	//Release the reference held by the class factory
	pNewComponent->NonDelegatingRelease();
	return hr;
}


//LockServer
HRESULT __stdcall CFactory::LockServer(BOOL bLock)
{
	if (bLock)
	{
		InterlockedIncrement(&s_cServerLocks);
	}
	else
	{
		InterlockedDecrement(&s_cServerLocks);
	}

	return S_OK;
}

//create class factory based on clsid

HRESULT CFactory::GetClassObject(const CLSID& clsid, const IID &iid, void **ppv)
{
	trace("GetClassObject called...");
	if ((iid != IID_IUnknown) && (iid != IID_IClassFactory))
	{
		return E_NOINTERFACE;
	}

	//Traverse the array of data looking for this class ID
	for (int i = 0; i < g_cFactoryDataEntries; i++)
	{
		const CFactoryData* pData = &g_FactoryDataArray[i];
		if (pData->IsClassID(clsid))
		{
			//if found clsid int the array of the components
			//pass the CFactoryData structure to the class factory
			//so that it knows what kind of components to create
			*ppv = (IUnknown*) new CFactory(pData);
			if (*ppv == NULL)
			{
				return E_OUTOFMEMORY;
			}
			return NOERROR;
		}
	}

	return CLASS_E_CLASSNOTAVAILABLE;
}

HRESULT CFactory :: RegisterAll()
{	
	trace("RegisteringAll : registering.....");
	HRESULT hr;
	for (int i = 0; i < g_cFactoryDataEntries; i++)
	{
		RegisterServer(s_hModule,
			*(g_FactoryDataArray[i].m_pCLSID),
			g_FactoryDataArray[i].m_RegistryName,
			g_FactoryDataArray[i].m_szVerIndProgID,
			g_FactoryDataArray[i].m_szProgID);
	}

	if (FAILED(hr))
	{
		trace("RegisterAll", hr);
	}
	else
	{
		trace("register success",hr);
	}

	return S_OK;
}


HRESULT CFactory::UnregisterAll()
{
	trace("UnRegisteringAll : unregistering.....");
	for (int i = 0; i < g_cFactoryDataEntries; i++)
	{
			UnregisterServer(*(g_FactoryDataArray[i].m_pCLSID),
			g_FactoryDataArray[i].m_szVerIndProgID,
			g_FactoryDataArray[i].m_szProgID);
	}

	return S_OK;
}


//Determine if the component can be unloaded
HRESULT CFactory::CanUnloadNow()
{
	if (CUnknown::ActiveComponents() || IsLocked())
	{
		return S_FALSE;
	}
	else
	{
		return S_OK;
	}
}


//Exported Functions				

STDAPI DllCanUnloadNow()
{
	return CFactory::CanUnloadNow();
}


//get Class factory

STDAPI DllGetClassObject(const CLSID &clsid, const IID &iid, void **ppv)
{
	trace("DllGetClassObject called");
	return CFactory::GetClassObject(clsid, iid, ppv);
}



//server registration

STDAPI DllRegisterServer(void)
{
	trace("DllRegisterServer called");
	return CFactory::RegisterAll();
}

STDAPI DllUnregisterServer(void)
{
	trace("DllUnRegisterServer called");

	return CFactory::UnregisterAll();
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void* lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		CFactory::s_hModule = (HMODULE)hModule;
	}

	return true;
}














































