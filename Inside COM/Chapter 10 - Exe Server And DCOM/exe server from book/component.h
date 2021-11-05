#include "iface.h"
#include "CUnknown.h"



//Component A

class CA : public CUnknown, public IX, public ISupportErrorInfo
{
public:
	//Creation
	static HRESULT CreateInstance(IUnknown* pUnknownOuter, CUnknown** ppNewComponent);
private:
	//declare the delegating IUnknown
	DECLARE_IUNKNOWN

	//IUnknown
	virtual HRESULT __stdcall NondelegatingQueryInterface(const IID &iid, void **ppv);

	//IDispatch 

};