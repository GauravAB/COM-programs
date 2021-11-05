#include "Component1.h"
#include "Util.h"
#include "CUnknown.h"
#include "Interface.h"


static inline  void trace(const char*msg)
{
	Util::Trace("Component1", msg, S_OK);
}
static inline void trace(const char*msg, HRESULT hr)
{
	Util::Trace("Component1", msg, hr);
}


//Interface ISum
HRESULT __stdcall CSumSubtract::SumOfTwoIntegers(int n1, int n2, int *n3)
{
	*n3 = n1 + n2;

	return S_OK;
}

//Interface ISubtract
HRESULT __stdcall CSumSubtract::SubtractionOfTwoIntegers(int n1, int n2, int *n3)
{
	*n3 = n1 - n2;
	return S_OK;
}

CSumSubtract::CSumSubtract(IUnknown* pUnknownOuter):CUnknown(pUnknownOuter),CSumSubtract::m_pUnknownInner(NULL), CSumSubtract::m_pISum(NULL), CSumSubtract::m_pISubtract(NULL)
{
	//nothing to do
}

CSumSubtract:: ~CSumSubtract(void)
{
	trace("destroy self");
}

HRESULT __stdcall CSumSubtract::NondelegatingQueryInterface(const IID &iid, void**ppv)
{
	if (iid == IID_IUnknown)
	{
		return FinishQI(static_cast<ISum*>(this), ppv);
	}
	else if (iid == IID_ISum)
	{
		return FinishQI(static_cast<ISum*>(this), ppv);	
	}
	else if (iid == IID_ISubtract)
	{
		return FinishQI(static_cast<ISubtract*>(this), ppv);
	}
	else
	{
		return CUnknown::NondelegatingQueryInterface(iid, ppv);
	}
}

HRESULT CSumSubtract::InitializeInnerComponent(void)
{
	//nothing for now
	return S_OK;
}

void CSumSubtract::FinalRelease(void)
{
	CUnknown::FinalRelease();
	GetOuterUnknown()->AddRef();

	m_pISum->Release();
	m_pISubtract->Release();

	if (m_pUnknownInner != NULL)
	{
		m_pUnknownInner->Release();
		m_pUnknownInner = NULL;
	}
}









