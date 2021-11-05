#include "iface.h"
#include "CUnknown.h"



//Component A

class CA : public CUnknown, public IX
{
public:
	//Creation
	static HRESULT CreateInstance(IUnknown* pUnknownOuter, CUnknown** ppNewComponent);
private:
	//declare the delegating IUnknown
	DECLARE_IUNKNOWN

	//IUnknown
	virtual HRESULT __stdcall NondelegatingQueryInterface(const IID &iid, void **ppv);

	//Interface IX
	virtual HRESULT __stdcall FxStringIn(wchar_t *szIn);
	virtual HRESULT __stdcall FxStringOut(wchar_t **pszOut);

	//Initialization
	virtual HRESULT InitializeInnerComponent();

	//NOtify derived classes we are releasing
	virtual void FinalRelease();

	//Constructor
	CA(IUnknown* pUnknownOuter);

	//Destructor
	~CA();

	//Pointer to inner object being aggregated

	IUnknown* m_pUnknownInner;

	//pointer to the interface supported
	IY* m_pIY;
};


