#include <windows.h>
#include "interface_inner.h"
#include "CFactory.h"


//globals

const char* g_szFriendlyName = "AggregationWithoutRegFileInnerServer";
const char* g_szVerIndProgID = "AggregationInnerServer";
const char* g_szProgID = "AggregationInnerServer_1.0";
static long g_cComponents;
static long g_cServerLocks;
HMODULE g_hModule;


interface INondelegatingUnknown
{
	virtual HRESULT __stdcall NondelegatingQueryInterface(const IID &iid, void**ppv0) = 0;
	virtual ULONG __stdcall NondelegatingAddRef(void)=0;
	virtual ULONG __stdcall NondelegatingRelease(void)=0;
};

class CB : public INondelegatingUnknown, public IY
{
public:
	//NondelegatingIUnknowns Methods
	virtual HRESULT __stdcall NondelegatingQueryInterface(const IID &iid, void**ppv);
	virtual ULONG __stdcall NondelegatingAddRef(void);
	virtual ULONG __stdcall NondelegatingRelease(void);

	//IUnknownMethods
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void**ppv);
	virtual	ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);
	
	virtual HRESULT __stdcall Fy(void);

	//class methods
	CB(IUnknown* pUnknownOuter);
	~CB(void);
private:
	long m_cRef;
	IUnknown* m_pIUnknownOuter;
};








