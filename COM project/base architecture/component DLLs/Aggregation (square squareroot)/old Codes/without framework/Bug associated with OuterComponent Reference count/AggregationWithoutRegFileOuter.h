//CLASS DECLARATION FOR OUTER COMPONENT
#include <Windows.h>
#include "interface.h"
#include "interface_inner.h"

static long g_cComponents;
static long g_cServerLocks;
HMODULE g_hModule;

const char* g_szFriendlyName = "AggregationWithoutRegFileOuterComponent";
const char* g_szVerIndProgID = "AggregationOuterComponent";
const char* g_szProgID = "AggregationOuterComponent_1.0";

//core class A Inheriting from interface IX
class CA : public IX 
{
public:
	//methods of IUnknown to implement
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//methods of interface IX
	virtual HRESULT __stdcall Fx(void);
	//class constructor and destructor
	CA();
	~CA();
	//Initialize inner component for aggregation
	virtual HRESULT __stdcall InitializeInnerComponent(void);

private:
	long m_cRef;
	IUnknown *m_pIUnknownInner;
};


