#include "iface.h"
#include "CUnknown.h"



//Component C

class CC : public CUnknown, public IZ
{
public:
	//Creation
	static HRESULT CreateInstance(IUnknown* pUnknownOuter, CUnknown** ppNewComponent);
private:
	//declare the delegating IUnknown
	DECLARE_IUNKNOWN

	//IUnknown
	virtual HRESULT __stdcall NondelegatingQueryInterface(const IID &iid, void **ppv);

	//Interface IZ
	virtual HRESULT __stdcall FzStructIn(Point3d pt);
	virtual HRESULT __stdcall FzStructOut(Point3d* pt);

	
	//Constructor
	CC(IUnknown* pUnknownOuter);

	//Destructor
	~CC();

	//Member variable
	Point3d m_pt;
	
};


