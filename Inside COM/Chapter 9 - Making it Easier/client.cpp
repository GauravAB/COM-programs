#include<iostream>
#include "interface.h"
#include "REGISTRY.H"

using namespace std;

void trace(const char * msg) { cout << msg << endl; }


int main()
{
	
	HMODULE hMod;
	typedef HRESULT(*pFn)(void);
	pFn pRegister = NULL;
	//initialize COM library
	CoInitialize(NULL);

	//to do 
	//set access permissions programmatically?

	
	//Register the dll
	//first load the library
	hMod = LoadLibrary("Server.dll");

	if (hMod == NULL)
	{
		trace("Failed to Load the DLL exitting now...");
		return 1;
	}
	else
	{
		trace("Loaded library");
	}

	//call the register function
	pRegister = (pFn)GetProcAddress(hMod,"DllRegisterServer");


	
	if (pRegister == NULL)
	{
		trace("GetProcAddress: Failed..exitting now...");
		return 1;
	}
	else
	{
		trace("DllRegisterServer Found");
	}
	pRegister();
	

	//Call CoCreateInstance to create component and get interface
	IX* pIX = NULL;
	HRESULT hr = CoCreateInstance(CLSID_Component1, NULL, CLSCTX_INPROC_SERVER, IID_IX,(void**)&pIX);
	if (SUCCEEDED(hr))
	{
		trace("Succeeded getting IX.");
		pIX->Fx();						//use interface
		trace("Asking for interface IY from IX.");
		IY *pIY = NULL;
		hr = pIX->QueryInterface(IID_IY, (void**)&pIY);
		if (SUCCEEDED(hr))
		{
			trace("Success in getting interface IY.");
			pIY->Fy();									//use interface
			pIY->Release();
			trace("Release IY after use.");
		}
		else
		{
			trace("Could not get interface IY");
		}

		trace("Ask for interface IZ.");
		IZ* pIZ = NULL;
		hr = pIX->QueryInterface(IID_IZ, (void**)&pIZ);
		if (SUCCEEDED(hr))
		{
			trace("Success in getting interface IZ.");
			pIZ->Fz();
			pIZ->Release();
			trace("Release IZ after use.");
		}
		else
		{
		}
		pIX->Release();

	}
	else
	{
		void *pMsgBuf;

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pMsgBuf, 0, NULL);
		cout << "Error (" << hex << hr << "):" << (LPTSTR)pMsgBuf << endl;

		cout << "Client could not create component" << endl;
	}

	//Uninitialize COM library
	CoUninitialize();
	return 0;
}



