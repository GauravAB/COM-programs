#include <iostream>
#include <Windows.h>


void trace(const char *msg) { std::cout << msg << std::endl; }

interface IX : IUnknown
{
	virtual void __stdcall fX() = 0;
};

interface IY : IUnknown
{
	virtual void __stdcall fY() = 0;
};

interface IZ : IUnknown
{
	virtual void __stdcall fZ() = 0;
};


extern const IID IID_IX;
extern const IID IID_IY;
extern const IID IID_IZ;

class CA : public IX, public IY
{

	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);
	
	virtual void __stdcall fX() { std::cout << "fX" << std::endl; }
	virtual void __stdcall fY() { std::cout << "fY" << std::endl; }
	virtual void __stdcall fZ() { std::cout << "fZ" << std::endl; }

public:	
	CA(void)
	{
		m_cRef = 0;
	}
	~CA(void)
	{
		trace("Destroyed Component");
		//nothing
	}

private:
	long m_cRef;
};

HRESULT __stdcall CA :: QueryInterface (const IID &iid ,  void** ppv)
{
	if (iid == IID_IUnknown)
	{
		trace("QueryInterface:	Return IUnknown Interface pointer");
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IX)
	{
		trace("QueryInterface:	Return IX Interface pointer");
		*ppv = static_cast<IX*>(this);
	}
	else if(iid == IID_IY)
	{
		trace("QueryInterface:	Return IY Interface pointer");
		*ppv = static_cast<IY*>(this);
	}
	else
	{
		trace("QueryInterface:	Interface Not supported");
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return (S_OK);
}

ULONG __stdcall CA :: AddRef(void)
{
	std::cout<<"AddRef: Interface count incremented" << m_cRef + 1 << "."<<std::endl;
	return (InterlockedIncrement(&m_cRef));

}

ULONG __stdcall CA::Release(void)
{
	
	std::cout<<"Interface count decremented" << m_cRef - 1 << "."<<std::endl;

	if ((InterlockedDecrement(&m_cRef)) == 0)
	{
		trace("Closing the component");
		delete(this);
		return (0);
	}

	return(S_OK);
	
}

IUnknown* CreateInstance(void)
{
	IUnknown *pI = static_cast<IX*>(new CA);
	pI->AddRef();
	return(pI);
}


// {81DDB274-F9E5-4542-B2B6-004852B5B036}
static const IID IID_IX = { 0x81ddb274, 0xf9e5, 0x4542,{ 0xb2, 0xb6, 0x0, 0x48, 0x52, 0xb5, 0xb0, 0x36 } };

// {9EB2C88B-9687-4556-8E5B-33432A31B525}
static const IID IID_IY = { 0x9eb2c88b, 0x9687, 0x4556,{ 0x8e, 0x5b, 0x33, 0x43, 0x2a, 0x31, 0xb5, 0x25 } };

// {F3D6B742-9879-4B12-BCEF-3CC9A619C04E}
static const IID IID_IZ = { 0xf3d6b742, 0x9879, 0x4b12,{ 0xbc, 0xef, 0x3c, 0xc9, 0xa6, 0x19, 0xc0, 0x4e } };



int main(int argc, char *argv[])
{
	HRESULT hr;

	trace("Client started");
	trace("Getting Interface Pointer");
	IUnknown *pIUnknown = CreateInstance();

	trace("Getting IX from IUnknown");

	IX* pIX = NULL;

	hr = pIUnknown->QueryInterface(IID_IX, (void**)&pIX);
	
	if (SUCCEEDED(hr))
	{
		trace("succeeded to get IX pointer");
		pIX->fX();
		pIX->Release();
	}

	pIUnknown->Release();

	return 0;
}





