#include <Windows.h>
#include <iostream>
#include "iface.h"
#include "registry.h"
#include <stdlib.h>

using namespace std;
void trace(const char *msg) { cout << "Client" << msg << endl; }


int main(void)
{

	CoInitialize(NULL);

	trace("Get IX interface pointer from component 1.");
	IX *pIX = NULL;
	HRESULT hr = CoCreateInstance(CLSID_Component1, NULL, CLSCTX_INPROC_SERVER, IID_IX, (void**)&pIX);
	
	
	if (FAILED(hr))
	{
		trace("Client: CoCreateInstance failed");	
		exit(1);
	}

	if(SUCCEEDED(hr))
	{
		trace("Success in getting IX pointer");
		pIX->Fx();
	}


	return 0;
}