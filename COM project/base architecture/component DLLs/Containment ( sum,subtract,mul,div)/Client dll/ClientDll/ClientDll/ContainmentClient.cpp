#include <Windows.h>
#include "register.h"
#include "Util.h"
#include <iostream>
#include "ContainmentClient.h"
#include "ContainmentOuterServer.h"

//globals
HMODULE ghModule;
TCHAR winSysPath[MAX_PATH];
ISum* pISum = NULL;
ISub* pISub = NULL;
IMultiplication* pIMul = NULL;
IDivision* pIDiv = NULL;

void trace(const char *msg, HRESULT hr)
{
	Util::Trace("Client", msg, hr);
}

void trace(const char* msg)
{
	Util::Trace("Client", msg, S_OK);
}


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	HRESULT hr_COMLIB = NULL;
	HRESULT hr = NULL;

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		hr_COMLIB = CoInitialize(NULL);	
		InitializeRegisterServer();

		//outer server init
		hr = CoCreateInstance(CLSID_CSumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void**)&pISum);
		if (SUCCEEDED(hr))
		{
			
			hr = pISum->QueryInterface(IID_ISub,(void**)&pISub);
			if (FAILED(hr))
			{
				trace("ISub Query Failed");
			}
			else
			{
				hr = pISub->QueryInterface(IID_IMul, (void**)&pIMul);
				if (FAILED(hr))
				{
					trace("failed to get IMul from pISub");
				}
				else
				{
					hr = pIMul->QueryInterface(IID_IDiv, (void**)&pIDiv);
					if(FAILED(hr))
					{
						trace("failed to get IDiv from IMul");
					}
				}
			}
		}
		else
		{
			//trace("CoCreateInstance Failed",hr);
		}

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		if (pISub)
		{
			pISub->Release();
		}
		if (pISum)
		{
			pISum->Release();
		}
		if (pIMul)
		{
			pIMul->Release();
		}
		if (pIDiv)
		{
			pIDiv->Release();
		}

		if (hr_COMLIB == S_OK)
		{
			CoUninitialize();
		}
		UninitializeRegisterServer();
	}

	return TRUE;
}



void InitializeRegisterServer(void)
{
	TCHAR str[255];
	GetSystemDirectory(winSysPath, sizeof(winSysPath));

	wsprintf(str, L"%s\\regsvr32.exe -s ..\\serverLibraries\\ContainmentOuterServer.dll", winSysPath);
	if (MyCreateProcess(str) == 0)
	{
		trace("InitializeRegisterServer Failed");
	}

	wsprintf(str, L"%s\\regsvr32.exe -s ..\\serverLibraries\\ContainmentInnerServer.dll", winSysPath);
	if (MyCreateProcess(str) == 0)
	{
		trace("InitializeRegisterServer Failed");
	}

	

}


void UninitializeRegisterServer(void)
{
	TCHAR str[255];
	GetSystemDirectory(winSysPath, sizeof(winSysPath));
	wsprintf(str, L"%s\\regsvr32.exe -s -u ..\\serverLibraries\\ContainmentInnerServer.dll", winSysPath);
	if (MyCreateProcess(str) == 0)
	{
		//trace("UnInitializeRegisterServer Inner Failed");
	}

	wsprintf(str, L"%s\\regsvr32.exe -s -u  ..\\serverLibraries\\ContainmentOuterServer.dll", winSysPath);
	if (MyCreateProcess(str) == 0)
	{
		//trace("UnitializeRegisterServer Outer Failed");
	}

}

int MyCreateProcess(TCHAR *szPath)
{
	STARTUPINFO stInfo;
	PROCESS_INFORMATION pInfo;

	ZeroMemory(&stInfo, sizeof(stInfo));
	ZeroMemory(&pInfo, sizeof(pInfo));

	stInfo.cb = sizeof(STARTUPINFO);
	stInfo.dwFlags = STARTF_USESHOWWINDOW;
	stInfo.wShowWindow = SW_SHOWDEFAULT;


	if (0 == CreateProcess(NULL, szPath, NULL, NULL, NULL, NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo))
	{
		//trace("CreateProcess failed");
		return 0;
	}
	else
	{
		WaitForInputIdle(pInfo.hProcess, INFINITE);
	}
	
	return 1;
}


STDAPI DllCallSumOfTwoIntegers(int n1, int n2, int*n3)
{
	return pISum->additionOfTwoIntegers(n1, n2, n3);
}

STDAPI DllCallSubtractionOfTwoIntegers(int n1, int n2, int* n3)
{
	return pISub->subtractionOfTwoIntegers(n1, n2, n3);
}

STDAPI DllCallMultiplicationOfTwoIntegers(int n1 , int n2,  int* n3)
{
	return pIMul->multiplicationOfTwoIntegers(n1,n2,n3);
}

STDAPI DllCallDivisionOfTwoIntegers(int n1 , int n2 , int* n3)
{
	return pIDiv->divisionOfTwoIntegers(n1, n2, n3);
}









