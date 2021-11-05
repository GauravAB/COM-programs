#include <iostream>
#include <Windows.h>

using namespace std;

void trace(const char *msg) { cout << msg << endl; }

//simulating COM interfaces
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

//forward reference for unique IDs
 const IID IID_IX;
 const IID IID_IY;
 const IID IID_IZ;


//Component 

class CA : public IX, public IY
{

	//UIknown methods
	virtual HRESULT __stdcall QueryInterface(const IID& iid ,  void** ppv);
	virtual ULONG __stdcall AddRef(void) { return 0; }
	virtual ULONG __stdcall Release(void) { return 0; }

	//methods of the interfaces

	virtual void __stdcall fX() { cout << "fX" << endl; }

	virtual void __stdcall fY() { cout << "fY" << endl; }
};

//Creation function
IUnknown* CreateInstance()
{
	IUnknown *pI = static_cast<IX*>(new CA);
	pI->AddRef();
	return(pI);
}



HRESULT __stdcall CA::QueryInterface(const IID &iid, void** ppv)
{
	if (iid == IID_IUnknown)
	{
		trace("QueryInterface: return pointer to IUnknown");
		*ppv = static_cast<IX*>(this);
	}
	else if( iid == IID_IX)
	{
		trace("QueryInterface: Return pointer to IX");
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IY)
	{
		trace("QueryInterface: Return pointer to IY");
		*ppv = static_cast<IY*>(this);
	}
	else
	{
		trace("QueryInterface: Interface not supported");
		*ppv = NULL;

		return E_NOINTERFACE;
	}

	static_cast<IUnknown*>(*ppv)->AddRef();

	return S_OK;
}

//alocation IDs to the interfaces
// {AD026C07-5CAA-4D0B-9989-C801A6881450}
static const IID IID_X = { 0xad026c07, 0x5caa, 0x4d0b,{ 0x99, 0x89, 0xc8, 0x1, 0xa6, 0x88, 0x14, 0x50 } };

// {14757898-FAE9-4660-950A-D9803779450C}
static const IID IID_Y = { 0x14757898, 0xfae9, 0x4660,{ 0x95, 0xa, 0xd9, 0x80, 0x37, 0x79, 0x45, 0xc } };

// {23DC8F54-F12E-43A4-809E-9E6227EEC05F}
static const IID IID_Z = { 0x23dc8f54, 0xf12e, 0x43a4,{ 0x80, 0x9e, 0x9e, 0x62, 0x27, 0xee, 0xc0, 0x5f } };


//client
int main(void)
{

	HRESULT hr;
	trace("Client: Get an IUnknown Pointer .");
	IUnknown *pIUnknown = CreateInstance();

	
	trace("Client: Get Interface IX");
	IX* pIX = NULL;

	hr = pIUnknown->QueryInterface(IID_IX, (void**)&pIX);
	if (SUCCEEDED(hr))
	{
		trace("Client: Success getting IX.");
		pIX->fX();		//use interface
	}

	trace("Client: Get Interface IY");
	IY* pIY = NULL;
	hr = pIUnknown->QueryInterface(IID_IY, (void**)&pIY);

	if (SUCCEEDED(hr))
	{
		trace("Client: Success getting IY.");
		pIY->fY();	//use the interface
	}

	trace("Client: Get Interface IZ");
	IZ* pIZ = NULL;
	hr = pIUnknown->QueryInterface(IID_IZ, (void**)&pIZ);
	if (SUCCEEDED(hr))
	{
		trace("Client: Success getting IZ");
		pIZ->fZ();
	}
	else
	{
		trace("Client: Could not get the interface IZ");
	}


	trace("Client: Get interface IY from IX");
	IY *pIYfromIX = NULL;

	hr = pIX->QueryInterface(IID_IY, (void**)&pIYfromIX);
	if (SUCCEEDED(hr))
	{
		trace("Client: Success getting pIY from pIX");
		pIYfromIX->fY();
	}
	
	trace("Client: Interface IUnknown from IY ");
	IUnknown *pIUnknownfromIY = NULL;
	
	hr = pIY->QueryInterface(IID_IUnknown, (void**)&pIUnknownfromIY);

	if (SUCCEEDED(hr))
	{
		trace("Client: Success in getting pIUnknown from pIY");
		trace("Let us check whether the Poiters are equal");

		if (pIUnknownfromIY == pIUnknown)
		{
			cout << "Yes" << endl;
		}
		else
		{
			cout << "IUnkownfrompIY != IUnkown" << endl;
		}
	}

	//Delete the component

	delete pIUnknown;

	return 0;
}



