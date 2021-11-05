#include "CUnknown.h"
#include "interface.h"


class CC : public CUnknown, public IZ
{
public:
	static HRESULT CreateInstance(IUnknown* pUnknown, CUnknown**ppNewComponent);
	
private:
	DECLARE_IUNKNOWN

	virtual HRESULT __stdcall NonDelegatingQueryInterface(const IID &iid, void **ppv);

	virtual void __stdcall Fz();

	//Constructor
	CC(IUnknown* pUnknownOuter);

	~CC();
};
