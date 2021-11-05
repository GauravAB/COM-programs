#include <iostream>
#include <Windows.h>


void trace(const char *msg) { std::cout << msg << std::endl; }


interface IX : IUnknown
{
	virtual void __stdcall fX() = 0;
};

interface IY :IUnknown
{
	virtual void __stdcall fY() = 0;
};

interface IZ :IUnknown
{
	virtual void __stdcall fZ() = 0;
};


extern const IID IID_IX;
extern const IID IID_IY;
extern const IID IID_IZ;


class CA : public IX, public IY
{
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef() { return 0; }
	virtual ULONG __stdcall Release() { return 0; }
	
	//interface
	virtual void __stdcall fY() { std::cout << "fY" << std::endl; }
	virtual void __stdcall fX() { std::cout << "fX"<<std::endl; }
};


HRESULT __stdcall CA::QueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IUnknown)
	{
		trace("QueryInterface: Return pointer to IX.");
		*ppv = static_cast<IX*>(this);
	}
	else if(iid == IID_IX)
	{
		trace("QueryInterface: Return Pointer to IX.");
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IY)
	{
		trace("QueryInterface: Return Pointer to IY.");
		*ppv = static_cast<IY*>(this);
	}
	else
	{
		trace("QueryInterface: Cannot match the given IID...");
		*ppv = NULL;
		return (E_NOINTERFACE);
	}

	static_cast<IUnknown*>(*ppv)->AddRef();
	return (S_OK);
}

//Creation function

IUnknown *CreateInstance()
{
	IUnknown *pI = static_cast<IX*>(new CA);
	pI->AddRef();
	return pI;
}

// {81DDB274-F9E5-4542-B2B6-004852B5B036}
static const IID IID_IX = { 0x81ddb274, 0xf9e5, 0x4542,{ 0xb2, 0xb6, 0x0, 0x48, 0x52, 0xb5, 0xb0, 0x36 } };

// {9EB2C88B-9687-4556-8E5B-33432A31B525}
static const IID IID_IY = { 0x9eb2c88b, 0x9687, 0x4556,{ 0x8e, 0x5b, 0x33, 0x43, 0x2a, 0x31, 0xb5, 0x25 } };

// {F3D6B742-9879-4B12-BCEF-3CC9A619C04E}
static const IID IID_IZ = { 0xf3d6b742, 0x9879, 0x4b12,{ 0xbc, 0xef, 0x3c, 0xc9, 0xa6, 0x19, 0xc0, 0x4e } };


//client

int main()
{
	HRESULT hr;
	trace("Client:	Get an IUnkown Pointer .");
	IUnknown *pIUnknown = CreateInstance();

	trace("Client:	Get interface IX.");
	IX *pIX = NULL;
	hr = pIUnknown->QueryInterface(IID_IX, (void**)&pIX);

	if (SUCCEEDED(hr))
	{
		trace("Client: succeeded getting IX");
		pIX->fX();
	}


	trace("Client:	Get interface IY.");
	IY* pIY = NULL;
	hr = pIUnknown->QueryInterface(IID_IY, (void**) &pIY);
	if (SUCCEEDED(hr))
	{
		trace("Client:	succeeded getting IY");
		pIY->fY();
	}

	trace("Client: Get interface IZ.");
	IZ* pIZ = NULL;
	hr = pIUnknown->QueryInterface(IID_IZ, (void**)&pIZ);
	if (SUCCEEDED(hr))
	{
		trace("Client:	succeeded getting IZ");
		pIZ->fZ();
	}
	else
	{
		trace("Client: failed to get IZ interface");
	}

	trace("Client: Get interface IY from IX .");
	IY* pIYfromIX =NULL;
	hr = pIX->QueryInterface(IID_IY, (void**)&pIYfromIX);

	if (SUCCEEDED(hr))
	{
		trace("Client: succeeded getting pIY from pIX .");
		pIYfromIX->fY();
	}

	trace("Client: Get pointer to IUnknown from pIY .");

	IUnknown *pIUnknownfromIY = NULL;
	hr = pIY->QueryInterface(IID_IUnknown, (void**)&pIUnknownfromIY);

	if(SUCCEEDED(hr))
	{ 
		trace("Client:	succeeded to Get pointer to IUnknown from pIY");
		trace("Client:	Let us check if the pointers are indeed the same ?.");

		if (pIUnknown == pIUnknownfromIY)
		{
			trace("YES . Both the pointers point to same IUnknown");
		}
		else
		{
			trace("NO . Some error");
		}
	}

	trace("Client Release IUnknown interface");
	pIUnknown->Release();
	delete(pIUnknown);
	return 0;
}





