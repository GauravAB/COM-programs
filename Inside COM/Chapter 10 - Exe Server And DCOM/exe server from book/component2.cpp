#include <iostream>
#include "iface.h"
#include "util.h"
#include "CUnknown.h"
#include "component2.h"

static inline void trace(char * msg)
{
	Util::Trace("Component2:", msg, S_OK);
}

static inline void trace(char*msg, HRESULT hr)
{
	Util::Trace("Component2:", msg, hr);
}

HRESULT __stdcall CB::FyCount(long* sizeArray)
{
	*sizeArray = m_sizeArray;
	return S_OK;
}

HRESULT __stdcall CB::FyArrayIn(long sizeIn , long arrayIn[])
{
	//Display the array passed in.
	
	std::cout<<"FyArrayIn revceived"<<sizeIn <<"elements: ";
	for(int i = 0 ; i <sizeIn; i++)
	{
		std::cout<<arrayIn[i];
	}

	std::cout <<"."<<std::endl;

	delete [] m_array;

	//create new array
	m_array = new long[sizeIn];
	m_sizeArray = sizeIn;

	//copy the 	array passed in into the internal array
	for (int i = 0 ; i <sizeIn ; i++)
	{
		m_array[i] = arrayIn[i];
	}
	return S_OK;
}


HRESULT __stdcall CB::FyArrayOut(long* psizeInOut , long arrayOut[])
{
	if(*psizeInOut < m_sizeArray)
	{
		return E_OUTOFMEMORY;
	}

	//copy the array to the out parameter
	for(int i=0 ; i<m_sizeArray; i++)
	{

		arrayOut[i] = m_array[i];
	}

	//return size of the array in the in/out parameter
	*psizeInOut = m_sizeArray;
	return S_OK;
}

CB::CB(IUnknown* pUnknownOuter):CUnknown(pUnknownOuter),m_pUnknownInner(NULL),m_pIZ(NULL)
{
	m_sizeArray = 0;
	m_array = NULL;
}

CB::~CB()
{
	if(m_array != NULL)
	{
		delete[] m_array;
	}

	trace("Destroy self");
}



HRESULT __stdcall CB::NondelegatingQueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IY)
	{
		return FinishQI(static_cast<IY*>(this), ppv);
	}
	else if (iid == IID_IZ)
	{
		trace("return IZ component of aggregated component");
		return m_pUnknownInner->QueryInterface(iid, ppv);
	}
	else
	{
		return CUnknown::NondelegatingQueryInterface(iid, ppv);
	}
}


//Initialze the component by creating the contained component
HRESULT CB::InitializeInnerComponent()
{
	trace("Create Component3 , which is aggregated.");
	HRESULT hr = CoCreateInstance(CLSID_Component3,GetOuterUnknown(), CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&m_pUnknownInner);
	

	if (FAILED(hr))
	{
		trace("could not create inner component", hr);
		return E_FAIL;
	}

	trace("get pointer to interface IZ for cache");
	hr = m_pUnknownInner->QueryInterface(IID_IZ, (void**)&m_pIZ);
	if (FAILED(hr))
	{
		trace("inner component does not support IZ", hr);
		m_pUnknownInner->Release();
		m_pUnknownInner = NULL;
		return E_FAIL;
	}

	
	trace("Got IZ interface pointer . Release reference.");
	GetOuterUnknown()->Release();
	return S_OK;
}

void CB::FinalRelease()
{
	CUnknown::FinalRelease();
	GetOuterUnknown()->AddRef();
	m_pIZ->Release();

	if (m_pUnknownInner != NULL)
	{
		m_pUnknownInner->Release();
	}
}


HRESULT CB::CreateInstance(IUnknown* pUnknownOuter,CUnknown** ppNewComponent)
{
	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	*ppNewComponent = new CB(pUnknownOuter);
	return S_OK;
}












