#include <windows.h>
#include "interface_inner.h"
#include "interface_outer.h"


//Globals 
static long g_cComponents = 0;
static long g_cServerLocks = 0;
static HMODULE g_hModule = NULL;
const char *g_szFriendlyName = "SumSubtractProviderOuterComponent";
const char *g_szVerIndProgID = "ContainmentWithoutRegfile";
const char *g_szProgID = "ContainmentWithoutRegFile_1.0";


class CSumSubtract :public ISum, public ISubtract , public IMultiply , public IDivide
{
public:
	//IUnknown methods to be implemented
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//Interface methods
	virtual void __stdcall SumOfTwoNumbers(int n1, int n2, int *n3);
	virtual void __stdcall SubtractionOfTwoNumbers(int n1, int n2, int*n3);
	virtual void __stdcall MultiplicationOfTwoNumbers(int n1, int n2, int*n3);
	virtual void __stdcall DivisionOfTwoNumbers(int n1, int n2, int*n3);

	HRESULT InitializeInnerComponent(void);

	CSumSubtract(void);
	~CSumSubtract(void);

private:
	long m_cRef;
	IMultiply *m_pIMultiply;
	IDivide *m_pIDivide;
};


class CFactory : public IClassFactory
{
public:
	//IUnknown methods to be implemented
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	virtual HRESULT __stdcall CreateInstance(IUnknown* pUnknownOuter, REFIID iid, void **ppv);
	virtual HRESULT __stdcall LockServer(BOOL fLock);
	
private:
	long m_cRef;
};




