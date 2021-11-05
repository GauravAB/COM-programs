#include "CUnknown.h"
#include "UTIL.h"

static inline void trace(const char *msg)
{
	Util::Trace("CUnknown",msg ,S_OK);
}

static inline void trace(const char *msg , HRESULT hr)
{
	Util::Trace("CUnknown",msg,hr);
}

long CUnknown::s_cActiveComponents = 0;



//CUnknown constructor sets the aggregation mode based on pUnknownOuter
CUnknown::CUnknown(IUnknown* pUnknownOuter):m_cRef(1)
{
	if (pUnknownOuter == NULL)
	{
		trace("Not aggregating , delegate to Non-delegating IUnknown.");
		m_pUnknownOuter = reinterpret_cast<IUnknown*>(static_cast<INonDelegatingUnknown*>(this));
	}
	else
	{
		trace("Aggregating , delegate to outer delegating IUnknown ");
		m_pUnknownOuter = pUnknownOuter;
	}

	//increment count of active components
	InterlockedIncrement(&s_cActiveComponents);
}


CUnknown::~CUnknown()
{
	InterlockedDecrement(&s_cActiveComponents);
}

//Final release called by release before deleting the component

void CUnknown::FinalRelease()
{
	trace("Increment reference count for final release.");
	m_cRef = 1;
}

//NonDelegatin IUnknown
//--------@Override to handle custom interfaces

HRESULT __stdcall CUnknown::NonDelegatingQueryInterface(const IID& iid, void **ppv)
{
	//CUnknown only supports IUnknown

	if (iid == IID_IUnknown)
	{
		return FinishQI(reinterpret_cast<IUnknown*>(static_cast<INonDelegatingUnknown*>(this)), ppv);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
}

ULONG __stdcall CUnknown::NonDelegatingAddRef()
{
	return InterlockedIncrement(&m_cRef);
}

	
ULONG __stdcall	CUnknown::NonDelegatingRelease()
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		FinalRelease();
		delete this;
		return 0;
	}

	return m_cRef;
}


HRESULT CUnknown::FinishQI(IUnknown*pI, void**ppv)
{
	*ppv = pI;
	pI->AddRef();
	return S_OK;
}









