#include<Windows.h>
#include <iostream>
#include "Interface.h"
#include "Util.h"

void trace(const char* msg)
{
	Util::Trace("Client", msg, S_OK);
}

void trace(const char* msg,HRESULT hr)
{
	Util::Trace("Client", msg, hr);
}

int main(void)
{
	HRESULT hr;
	ISum* pISum = NULL;
	ISubtract* pISubtract = NULL;
	int n1, n2, n3;
	TCHAR WinSysDir[255];
	static TCHAR szPath[_MAX_PATH];


	GetSystemDirectory(WinSysDir, 255);
	sprintf(szPath, TEXT("%s\\regsvr32.exe ProxyStub.dll"), WinSysDir);
	//CreateProcessA(szPath, NULL, NULL, NULL, FALSE, DETACHED_PROCESS | NORMAL_PRIORITY_CLASS, NULL, NULL, NULL, NULL);
	WinExec(szPath, SW_SHOWDEFAULT);

	hr = CoInitialize(NULL);
	if (hr == S_FALSE)
	{
		std::cout << "failed to initialize COM Server" << std::endl;
		return 1;
	}


	hr = CoCreateInstance(CLSID_CSumSubtract, NULL, CLSCTX_LOCAL_SERVER, IID_ISum, (void**)&pISum);

	
	if (FAILED(hr))
	{
		trace("CoCreateInstance", hr);
		return 1;
	}
	else
	{
		n1 = 25;
		n2 = 25;

		pISum->SumOfTwoIntegers(n1, n2, &n3);
		std::cout << "n3:" << n3 << std::endl;
		
		pISum->QueryInterface(IID_ISubtract, (void**)&pISubtract);
		pISum->Release();
		pISubtract->SubtractionOfTwoIntegers(n1, n2, &n3);
		std::cout << "n3:" << n3 << std::endl;

		
	}


	CoUninitialize();

	return 0;
}