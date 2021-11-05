#include <Windows.h>
#include "Interface.h"
#include "CUnknown.h"


class CSumSubtract : public ISum, public ISubtract, public CUnknown
{
public:
	DECLARE_IUNKNOWN

	//IUnknown
	virtual HRESULT __stdcall NondelegatingQueryInterface(const IID &iid, void**ppv);

	//Interface member funcs to be implemented
	virtual HRESULT __stdcall SumOfTwoIntegers(int num1, int num2, int* num3);
	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int num1, int num2, int* num3);

	virtual HRESULT __stdcall InitializeInnerComponent(void);
	void FinalRelease(void);

	CSumSubtract(IUnknown* pUnknownOuter);
	~CSumSubtract(void);
private:
	IUnknown* m_pUnknownInner;

	ISum* m_pISum;
	ISubtract *m_pISubtract;
};


