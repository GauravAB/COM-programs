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

class CA : public IX, public IY, public IZ
{
private:
	long m_cRef;
public:
	
	CA() 
	{
		m_cRef = 0;
	}
	~CA()
	{
		trace("delete component");
	}

	virtual HRESULT __stdcall QueryInterface(IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//interface methods

	virtual void __stdcall fX() { trace("fX"); }
	virtual void __stdcall fY() { trace("fY"); }
	virtual void __stdcall fZ() { trace("fZ"); }

};

extern const IID IID_IX;
extern const IID IID_IY;
extern const IID IID_IZ;

HRESULT CA::QueryInterface(IID &iid, void **ppv)
{
	if (iid == IID_IUnknown)
	{
		trace("CA QueryInterface: Return pointer to IUnknown.");
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IX)
	{
		trace("CA QueryInterface: Return pointer to IX. ");
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IY)
	{
		trace("CA QueryInterface: return pointer to IY. ");
		*ppv = static_cast<IY*>(this);
	}
	else
	{
		trace("QueryInterface: Interface Unavailable");
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return(S_OK);
}

ULONG __stdcall CA::AddRef()
{
	std::cout << "CA:		AddRef = " << m_cRef + 1 << "." << std::endl;
	return InterlockedIncrement(&m_cRef);	
}

ULONG __stdcall CA::Release()
{
	std::cout << "CA:		Release = " << m_cRef - 1 << "." << std::endl;
	
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete(this);
		return 0;
	}

	return m_cRef;
}


//creation function
IUnknown* CreateInstance(void)
{
	IUnknown *pI = static_cast<IX*>(new CA);
	pI->AddRef();

	return(pI);
}

// {D028B95E-F2C4-4429-8A66-CE55A29FAAFC}
static const IID IID_X = { 0xd028b95e, 0xf2c4, 0x4429,{ 0x8a, 0x66, 0xce, 0x55, 0xa2, 0x9f, 0xaa, 0xfc } };

// {F8F4C372-2CD9-414F-8733-5DBC9B425E83}
static const IID IID_Y ={ 0xf8f4c372, 0x2cd9, 0x414f,{ 0x87, 0x33, 0x5d, 0xbc, 0x9b, 0x42, 0x5e, 0x83 } };

// {A4CC8D7D-54D1-4B1F-8254-7989127BF6A2}
static const IID IID_Z ={ 0xa4cc8d7d, 0x54d1, 0x4b1f,{ 0x82, 0x54, 0x79, 0x89, 0x12, 0x7b, 0xf6, 0xa2 } };


//client

int main(void)
{
	HRESULT hr;

	trace("Get IUnknown Pointer.");
	IUnknown *pIUnknown = CreateInstance();

	trace("Client: Get interface IX.");

	IX* pIX = NULL;
	hr = pIUnknown->QueryInterface(IID_X, (void**)&pIX);

	if ((SUCCEEDED(hr))
	{
		trace("Client: Succeeded getting IX. ");
		pIX->fX();
	}

	return 0;
}
