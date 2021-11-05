#include <Windows.h>
#include "register.h"
#include "Util.h"
#include <iostream>
#include "ClassFactoryClient.h"
#include "ClassFactoryServerWithoutRegFile.h"

//globals
HMODULE ghModule;
TCHAR winSysPath[MAX_PATH];
ISum* pISum = NULL;
ISubtract* pISub = NULL;

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

		hr = CoCreateInstance(CLSID_CSumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void**)&pISum);
		if (SUCCEEDED(hr))
		{
			hr = pISum->QueryInterface(IID_ISubtract, (void**)&pISub);
			if (FAILED(hr))
			{
				//trace("ISub Query Failed");
			}
		}
		else
		{
			//trace("CoCreateInstance Failed");
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

	wsprintf(str, L"%s\\regsvr32.exe -s ClassFactoryWithoutRegFile.dll", winSysPath);
	if (MyCreateProcess(str) == 0)
	{
		//trace("InitializeRegisterServer Failed");
	}

}


void UninitializeRegisterServer(void)
{
	TCHAR str[255];
	GetSystemDirectory(winSysPath, sizeof(winSysPath));
	wsprintf(str, L"%s\\regsvr32.exe -s -u ClassFactoryWithoutRegFile.dll", winSysPath);
	if (MyCreateProcess(str) == 0)
	{
		//trace("UnInitializeRegisterServer Failed");
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


	if (0 == CreateProcess(NULL, szPath, NULL, NULL, NULL, DETACHED_PROCESS | IDLE_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo))
	{
	//	trace("CreateProcess failed");
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
	return pISum->SumOfTwoIntegers(n1, n2, n3);
}

STDAPI DllCallSubtractionOfTwoIntegers(int n1, int n2, int* n3)
{
	return pISub->SubtractionOfTwoIntegers(n1, n2, n3);
}









