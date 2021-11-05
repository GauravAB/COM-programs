#include <iostream>
#include <Windows.h>
#include "IFACE.h"
#include "Create.h"

using namespace std;

void trace(const char * msg) { cout << msg << endl; }

int main(void)
{
	HRESULT hr;

	char name[40];

	cout << "Enter the filename of the component to use [Cmpnt?.dll]:" << endl;
	cin >> name;
	cout << endl;

	//Create componant by calling createInstance function in the DLL.

	trace("Client: Initiating process to Get the Interface pointer..");

	IUnknown *pIUnknown = callCreateInstance(name);

	if (pIUnknown == NULL)
	{
		trace("Call to CreateInstance failed");
		return 1;
	}

	trace("Client: Get interface IX.");

	IX* pIX = NULL;
	hr = pIUnknown->QueryInterface(IID_IX,(void**)&pIX);
	if (SUCCEEDED(hr))
	{
		trace("Client : IX Interface found");
		pIX->fX();
	}
	else
	{
		pIUnknown->Release();
		trace("Interface not found");
		return(1);
	}

	IY *pIY;
	IZ *pIZ;
	hr = pIX->QueryInterface(IID_IY,(void**) &pIY);

	if (SUCCEEDED(hr))
	{
		trace("Client: pIY found");
		pIY->fY();
	}
	else
	{
		pIUnknown->Release();
		trace("Interface not found");
		return(1);
	}

	hr = pIY->QueryInterface(IID_IZ, (void**)&pIZ);

	if (SUCCEEDED(hr))
	{
		trace("Client: pIZ found");
		pIZ->fZ();
	}
	else
	{
		pIUnknown->Release();
		trace("Interface not found");
		return(1);
	}

	pIX->Release();
	pIY->Release();
	pIZ->Release();

	pIUnknown->Release();

	return 0;
}
