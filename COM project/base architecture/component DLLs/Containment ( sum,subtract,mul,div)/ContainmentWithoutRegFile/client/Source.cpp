#include <Windows.h>
#include <stdio.h>
#include "ContainmentInnerServer.h"
#include "Util.h"


void trace(const char * msg)
{
	Util::Trace("client", msg, S_OK);
}


void trace(const char*msg, HRESULT hr)
{
	Util::Trace("client", msg, hr);
}


int main(void)
{
	HRESULT hr;
	IDivision* pIDiv;
	IMultiplication* pIMul;
	int n1, n2, n3;

	
	TCHAR sysPath[MAX_PATH];
	TCHAR str[255];
	
	STARTUPINFO stInfo;
	PROCESS_INFORMATION pInfo;
	ZeroMemory(&stInfo, sizeof(STARTUPINFO));
	
	stInfo.cb = sizeof(STARTUPINFO);
	stInfo.dwFlags = STARTF_USESHOWWINDOW;
	stInfo.wShowWindow = SW_SHOWDEFAULT;




	trace("Initiating COM server");
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		trace("CoInitialize failed");
	}

	GetSystemDirectory(sysPath, MAX_PATH);
	wsprintf(str, L"%s regsvr32.exe ContainmentInnerComponent.dll", sysPath);
	
	if (0 != CreateProcess(NULL, str, NULL, NULL, FALSE, DETACHED_PROCESS | NORMAL_PRIORITY_CLASS, NULL, NULL,&stInfo,&pInfo))
	{

		hr = CoCreateInstance(CLSID_CMulDiv, NULL, CLSCTX_INPROC_SERVER, IID_IMul, (void**)&pIMul);

		if (FAILED(hr))
		{
			trace("CoCreateInstance failed", hr);
			return 1;
		}
		else
		{
			n1 = 11;
			n2 = 12;
			pIMul->multiplicationOfTwoIntegers(n1, n2, &n3);
			pIMul->QueryInterface(IID_IDiv, (void**)&pIDiv);

			cout << "Multiplication of " << n1 << "and " << n2 << "is " << n3 << endl;
			n1 = 100;
			n2 = 2;
			pIDiv->divisionOfTwoIntegers(n1, n2, &n3);
			cout << "Division of " << n1 << "and " << n2 << "is " << n3 << endl;
		}
	}
	
	return 0;
}