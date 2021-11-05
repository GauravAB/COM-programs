#include <windows.h>
#include "interface_inner.h"


//Globals 
static long g_cComponents = 0;
static long g_cServerLocks = 0;
static HMODULE g_hModule = NULL;
const char *g_szFriendlyName = "SumSubtractProviderInnerComponent";
const char *g_szVerIndProgID = "ContainmentInnerServerWithoutRegfile";
const char *g_szProgID = "ContainmentInnerServerWithoutRegFile_1.0";


class CMultiplyDivide :public IMultiply, public IDivide
{
public:
	//IUnknown methods to be implemented
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//Interface methods
	virtual void __stdcall MultiplicationOfTwoNumbers(int n1, int n2, int *n3);
	virtual void __stdcall DivisionOfTwoNumbers(int n1, int n2, int*n3);

	CMultiplyDivide(void);
	~CMultiplyDivide(void);

private:
	long m_cRef;
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




