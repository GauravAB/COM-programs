#include "CUnknown.h"
#include "interface.h"

class CA : public IX , public CUnknown
{
public:
	static HRESULT CreateInstance(IUnknown *pUnknownOuter, CUnknown** ppNewComponent);

private:
	//implement Delegating IUnknown
	DECLARE_IUNKNOWN
	
	//NonDelegating IUnknown
	virtual HRESULT __stdcall NonDelegatingQueryInterface(const IID &iid, void **ppv);
	
	//Interface IX
	virtual void __stdcall Fx();				
	
	//Initialization
	virtual HRESULT	Init();


	//CleanUP 
	virtual void FinalRelease(void);

	CA(IUnknown* pUnknownOuter);
	~CA(void);

	long m_cRef;

	//Pointer to inner object
	IUnknown *m_pUnknownInner;

	//interface pointer to inner component
	IY* m_pIY;
};





