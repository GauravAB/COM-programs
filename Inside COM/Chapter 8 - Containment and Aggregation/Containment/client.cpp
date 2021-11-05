	#include <Windows.h>
#include <iostream>
#include "interfaces.h"

void trace(const char *msg) { std::cout << "Client:\t" << msg << std::endl; }

int main()
{
	CoInitialize(NULL);

	trace("Get interface IX from component 1.");

	IX* pIX = NULL;

	HRESULT hr = CoCreateInstance(CLSID_Component1, NULL, CLSCTX_INPROC_SERVER, IID_IX, (void**)&pIX);
	
	if (SUCCEEDED(hr))
	{
		trace("Succeeded in creating component.");
		pIX->Fx();

		trace("Get interface IY from IX.");
		IY *pIY = NULL;
		hr = pIX->QueryInterface(IID_IY, (void**)&pIY);
	
		if (SUCCEEDED(hr))
		{
			//pIX->Release();
			trace("Succeeded in getting IY");
			pIY->Fy();
			trace("Get interface IX from IY");
			IX *pIX2 = NULL;

			hr = pIY->QueryInterface(IID_IX, (void**)&pIX2);
			if (SUCCEEDED(hr))
			{
				trace("Success in getting IX from IY");
				pIX2->Fx();
				pIX2->Release();
			}
			else
			{
				trace("error should of got IX from IY");
			}

			pIY->Release();
		}
		pIX->Release();
	}
	else
	{
		trace("Failed to create component. exitting...");
	}
	
	

	CoUninitialize();

}