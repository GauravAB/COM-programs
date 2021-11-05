#include "interface.h"
#include "CUnknown.h"


class CB : public CUnknown , public IY
{
public:
	static HRESULT CreateInstance(IUnknown *pUnknownOuter, CUnknown**ppNewComponent);

private:
	//Declare delegating IUnknown
	DECLARE_IUNKNOWN
	
	//NonDelegating IUnknown
	virtual HRESULT __stdcall NonDelegatingQueryInterface(const IID &iid, void **ppv);

	//Interface IY
	virtual void __stdcall Fy();

	//Initialization
	virtual HRESULT Init(void);
		
	//cleanUP
	virtual void FinalRelease(void);
	
	//constructor
	CB(IUnknown* pUnknownOuter);
	//destructor
	~CB();

	//Pointer to inner object either contained or aggregated
	IUnknown *m_pUnknownInner;

	//pointer to interface supported by the component itself

	IZ* m_pIZ;
};






