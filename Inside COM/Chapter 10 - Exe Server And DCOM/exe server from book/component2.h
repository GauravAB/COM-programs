#include "iface.h"
#include "CUnknown.h"



//Component B

class CB : public CUnknown, public IY
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
	virtual HRESULT __stdcall FyCount(long* sizeArray);
	virtual HRESULT __stdcall FyArrayIn(long sizeIn,	long arrayIn[]);
	virtual HRESULT __stdcall FyArrayOut(long *psizeInOut,long arrayOut[]);



	//Initialization
	virtual HRESULT InitializeInnerComponent();

	//NOtify derived classes we are releasing
	virtual void FinalRelease();

	//Constructor
	CB(IUnknown* pUnknownOuter);

	//Destructor
	~CB();

	//Pointer to inner object being aggregated

	IUnknown* m_pUnknownInner;

	//pointer to the interface supported
	IZ* m_pIZ;

	//pointer to array
	long* m_array;
	//size of the array
	long m_sizeArray;
};


