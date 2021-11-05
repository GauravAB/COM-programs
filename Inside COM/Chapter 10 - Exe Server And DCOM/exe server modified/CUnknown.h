#ifndef __CUnknown_h__
#define __CUnknown_h__

#include "Util.h"

interface INondelegatingUnknown
{
	virtual HRESULT __stdcall NondelegatingQueryInterface(const IID&, void**) = 0;
	virtual ULONG __stdcall NondelegatingAddRef(void) = 0;
	virtual ULONG __stdcall NondelegatingRelease(void) = 0;
};

//CUnknown implements INondelegatingUnknown
class CUnknown : public INondelegatingUnknown
{
public:
	//from interface NondelegatingUnknown
	virtual HRESULT __stdcall NondelegatingQueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall NondelegatingAddRef(void);
	virtual ULONG __stdcall NondelegatingRelease(void);

	//Constructor
	CUnknown(IUnknown* pUnknownOuter);
	//Destructor
	~CUnknown(void);

	//Initialization especially for aggregates
	virtual HRESULT __stdcall InitializeInnerComponent(void) { return S_OK; }

	//Notification to derived classes that we are releasing
	virtual void FinalRelease(void);

	//Count the currently active components
	static long ActiveComponents() { return s_cActiveComponents; }

	//helper function
	HRESULT FinishQI(IUnknown* pI, void** ppv);

protected:
	//support for delegation
	IUnknown* GetOuterUnknown(void) const { return m_pUnknownOuter; }
private:
	//Reference count for this object
	long m_cRef;
	//pointer to external outer IUnknown
	IUnknown* m_pUnknownOuter;
	
	//Count of all active instances
	static long s_cActiveComponents;
};


//define delegating IUnknown

#define DECLARE_IUNKNOWN														\
virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv)			\
{																				\
	return GetOuterUnknown()->QueryInterface(iid,ppv);							\
};																				\
virtual ULONG __stdcall AddRef(void)											\
{																				\
	return GetOuterUnknown()->AddRef();											\
};																				\
virtual ULONG __stdcall Release(void)											\
{																				\
	return GetOuterUnknown()->Release();										\
};																				\

	



#endif
