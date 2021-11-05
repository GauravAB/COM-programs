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

	trace("Get interface IX.");

	IX* pIX = NULL;
	hr = pIUnknown->QueryInterface(IID_IX,(void**)&pIX);
	if (SUCCEEDED(hr))
	{
		trace("IX Interface found");
		pIX->fX();
		pIX->Release();
	}
	else
	{
		pIUnknown->Release();
		trace("Interface not found");
		return(1);
	}

	pIUnknown->Release();
}