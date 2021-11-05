#include <Windows.h>
#include "register.h"
#include "Util.h"
#include <iostream>
#include "Client.h"
#include "AutomationServer.h"


//globals
HMODULE ghModule;
TCHAR winSysPath[MAX_PATH];
ICubeCuberoot *pICubeCuberoot;

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
		hr = CoCreateInstance(CLSID_CCubeCubeRoot, NULL, CLSCTX_INPROC_SERVER, IID_ICubeCuberoot, (void**)&pICubeCuberoot);
		if (SUCCEEDED(hr))
		{	
			hr = pICubeCuberoot->QueryInterface(IID_ICubeCuberoot, (void**)&pICubeCuberoot);
			if (FAILED(hr))
			{
				trace("pICubeCuberoot failed to get");
			}
		}
		else
		{
			trace("CoCreateInstance Failed",hr);
		}

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		if (pICubeCuberoot)
		{
			pICubeCuberoot->Release();
		}
		
		UninitializeRegisterServer();
	}

	return TRUE;
}



void InitializeRegisterServer(void)
{
	TCHAR str[255];
	GetSystemDirectory(winSysPath, sizeof(winSysPath));

	wsprintf(str, L"%s\\regsvr32.exe -s  ..\\serverLibraries\\AutomationServer.dll", winSysPath);
	if (MyCreateProcess(str) == 0)
	{
		trace("InitializeRegisterServer Failed");
	}

	wsprintf(str, L"%s\\regsvr32.exe -s  ..\\serverLibraries\\AutomationProxyStub.dll", winSysPath);
	if (MyCreateProcess(str) == 0)
	{
		trace("InitializeRegisterServer PxyStb Failed");
	}
}


void UninitializeRegisterServer(void)
{
	TCHAR str[255];
	GetSystemDirectory(winSysPath, sizeof(winSysPath));
	wsprintf(str, L"%s\\regsvr32.exe -s -u  ..\\serverLibraries\\AutomationServer.dll", winSysPath);
	if (MyCreateProcess(str) == 0)
	{
		trace("UnInitializeRegisterServer Inner Failed");
	}

	wsprintf(str, L"%s\\regsvr32.exe -s -u   ..\\serverLibraries\\AutomationproxyStub.dll", winSysPath);
	if (MyCreateProcess(str) == 0)
	{
		trace("UnitializeRegisterServer Outer Failed");
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
		trace("CreateProcess failed");
		return 0;
	}
	else
	{
		WaitForInputIdle(pInfo.hProcess, INFINITE);
	}
	
	return 1;
}


STDAPI DllCallCubeOfNumber(int num, int* res)
{
	return pICubeCuberoot->CubeOfNumber(num, res);
}

STDAPI DllCallCubeRootOfNumber(int num, int* res)
{
	return pICubeCuberoot->CubeRootOfNumber(num, res);
}







