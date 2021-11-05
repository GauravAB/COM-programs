#ifndef __CUnknown_h___
#define __CUnknown_h___
#include <windows.h>


//NonDelegating IUnknown
interface INonDelegatingUnknown
{
	virtual HRESULT __stdcall NonDelegatingQueryInterface(const IID& iid, void **ppv) = 0;
	virtual ULONG __stdcall NonDelegatingAddRef() = 0;
	virtual ULONG __stdcall NonDelegatingRelease() = 0;
};

class CUnknown : public INonDelegatingUnknown
{
public:
	//NonDelegating IUnknown Implementation
	virtual HRESULT __stdcall NonDelegatingQueryInterface(const IID&, void** ppv);
	virtual ULONG __stdcall NonDelegatingAddRef();
	virtual ULONG __stdcall NonDelegatingRelease();

	//Constructor
	CUnknown(IUnknown *pUnknownOuter);
	//Destructor
	virtual ~CUnknown();

	//Initialization especially to access inner classes either by aggregation or containment
	virtual HRESULT Init() { return S_OK; }

	//Notification to derived class that we are releasing
	virtual void FinalRelease(void);

	//Count of Currently active components
	static long ActiveComponents(void) { return s_cActiveComponents; }
	
	//Helper Function
	HRESULT FinishQI(IUnknown* pI, void **ppv);

protected:
	//support for delegation
	IUnknown* GetOuterUnknown(void) const { return m_pUnknownOuter; }

private:
	//Reference count for this object
	long m_cRef;
	//Pointer to external outer IUnknown
	IUnknown *m_pUnknownOuter;

	//Count of all active instances
	static long s_cActiveComponents;
};


#define DECLARE_IUNKNOWN												\
virtual HRESULT __stdcall QueryInterface(const IID & iid, void **ppv)	\
{																		\
	return GetOuterUnknown()->QueryInterface(iid, ppv);					\
}																		\
virtual ULONG __stdcall AddRef()										\
{																		\
	return GetOuterUnknown()->AddRef();									\
}																		\
virtual ULONG __stdcall Release()										\
{																		\
	return GetOuterUnknown()->Release();								\
}																		\

#endif
