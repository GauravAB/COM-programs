#include <iostream>
#include "IFACE.h"
#include <Windows.h>

using namespace std;

void trace(const char *msg) { cout << msg << endl; }

//
//component
//

class CA : public IX
{
	//IUnknown Implementation
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//Interface IX implementation
	virtual void __stdcall fX() { cout << "fX" << endl; }
	
public:
	//constructor
	CA() : m_cRef(0){}

	//Destructor
	~CA()
	{
		trace("Destroy Component");
	}
private:
	long m_cRef;
};

HRESULT __stdcall CA :: QueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IX)
	{
		*ppv = static_cast<IX*>(this);
	}
	else 
	{
		trace("Interface not supported.");
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	static_cast<IUnknown*>(*ppv)->AddRef();

	return S_OK;
}

ULONG __stdcall CA::AddRef(void)
{
	return(InterlockedIncrement(&m_cRef));
}

ULONG __stdcall CA::Release(void) 
{
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete(this);
		return 0;
	}

	return m_cRef;
}

extern "C" IUnknown* CreateInstance()
{
	IUnknown *pI = static_cast<IX*>(new CA);
	pI->AddRef();
	return pI;
}








