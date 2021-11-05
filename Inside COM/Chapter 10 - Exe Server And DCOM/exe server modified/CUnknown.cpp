#include "Util.h"
#include "CUnknown.h"

static inline void trace(char*msg)
{
	Util::Trace("CUnknown", msg, S_OK);
}
static inline void trace(char*msg, HRESULT hr)
{
	Util::Trace("CUnknown", msg, hr);
}

//count of active components are used to determine whether we can unloaod the DLL/close server
long CUnknown::s_cActiveComponents = 0;


//Constructor

CUnknown::CUnknown(IUnknown* pUnknownOuter):m_cRef(1)
{
	//Ser m_pUnknownOuter pointer
	if (pUnknownOuter == NULL)
	{
		trace("Not aggregating delegate to nondelegating Unknown");
		m_pUnknownOuter = reinterpret_cast<IUnknown*>(static_cast<INondelegatingUnknown*>(this));
	}
	else
	{
		trace("aggregating delegate to outer Unknown");
		m_pUnknownOuter = pUnknownOuter;
	}

	::InterlockedIncrement(&s_cActiveComponents);
}


//Destructor

CUnknown::~CUnknown()
{
	::InterlockedDecrement(&s_cActiveComponents);
}

//Nondelegating IUnknown 
//Override to handle class specific interfaces

HRESULT __stdcall CUnknown::NondelegatingQueryInterface(const IID &iid, void **ppv)
{
	//CUnknown supports only IUnknown
	if (iid == IID_IUnknown)
	{
		return FinishQI(reinterpret_cast<IUnknown*>(static_cast<INondelegatingUnknown*>(this)), ppv);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
}


//AddRef

ULONG __stdcall CUnknown::NondelegatingAddRef(void)
{
	::InterlockedIncrement(&m_cRef);
	return m_cRef;
}

ULONG __stdcall CUnknown::NondelegatingRelease(void)
{
	::InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		FinalRelease();
		delete this;
		return 0;
	}

	return m_cRef;
}

HRESULT CUnknown::FinishQI(IUnknown* pI, void**ppv)
{
	*ppv = pI;
	pI->AddRef();
	return S_OK;
}

void CUnknown::FinalRelease(void)
{
	m_cRef = 1;
}





