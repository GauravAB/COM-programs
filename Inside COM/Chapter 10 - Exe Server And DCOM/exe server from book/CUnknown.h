#ifndef _CUNKNOWN_H_
#define _CUNKNOWN_H_
#include <windows.h>

interface INondelegatingUnknown
{
	virtual HRESULT __stdcall NondelegatingQueryInterface(const IID& iid, void **ppv) = 0;
	virtual ULONG __stdcall NondelegatingAddRef(void) = 0;
	virtual ULONG __stdcall NondelegatingRelease(void) = 0;

};


//Declaring CUnknown Base class to Implement IUnknown

class CUnknown : public INondelegatingUnknown
{
public:
	virtual HRESULT __stdcall NondelegatingQueryInterface(const IID& iid, void **ppv);
	virtual ULONG __stdcall NondelegatingAddRef(void);
	virtual ULONG __stdcall NondelegatingRelease(void);

	//Constructor
	CUnknown(IUnknown* pUnknownOuter);
	//Destructor
	~CUnknown(void);

	//Initialization function for aggregate components
	virtual HRESULT	InitializeInnerComponent(void) { return S_OK; }

	//Notify derived classes that we are releasing 
	virtual void FinalRelease(void);

	//Count of Currently active components
	static long ActiveComponents(void) { return s_cActiveComponents; }
	
	//helper function
	HRESULT FinishQI(IUnknown* pI, void **ppv);
protected:
	//Support for delegation
	IUnknown *GetOuterUnknown() const
	{
		return m_pUnknownOuter;
	}
private:
	//reference count for this object
	long m_cRef;

	//pointer to external (Outer) IUnknown
	IUnknown *m_pUnknownOuter;

	//Count of all active instances
	static long s_cActiveComponents;
};

#define DECLARE_IUNKNOWN												 \
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv) \
	{																	 \
		return GetOuterUnknown()->QueryInterface(iid , ppv);			 \
	};																	 \
	virtual ULONG __stdcall AddRef(void)								 \
	{																	 \
		return GetOuterUnknown()->AddRef();								 \
	};																	 \
	virtual ULONG __stdcall Release(void)								 \
	{																	 \
		return GetOuterUnknown()->Release();							 \
	};																	 \

#endif