#include <Windows.h>
#include "register.h"
#include "Util.h"
#include <iostream>
#include "ExeServerClient.h"
#include "Interface_exe_server.h"


ITrigo *pITrigo;

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
	//globals
	STARTUPINFO stInfo;
	PROCESS_INFORMATION pInfo;

	ZeroMemory(&stInfo, sizeof(stInfo));
	ZeroMemory(&pInfo, sizeof(pInfo));

	stInfo.cb = sizeof(STARTUPINFO);
	stInfo.dwFlags = STARTF_USESHOWWINDOW;
	stInfo.wShowWindow = SW_SHOWDEFAULT;


	if (dwReason == DLL_PROCESS_ATTACH)
	{
		trace("Client DLL Loaded in process address space");
		trace("registering server");
		InitializeRegisterServer();
		hr_COMLIB = CoInitialize(NULL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		trace("Client DLL getting unloaded from process address space");
		if (pITrigo)
		{
			pITrigo->Release();
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
	TCHAR winSysPath[MAX_PATH];
	TCHAR str[MAX_PATH];

	GetSystemDirectory(winSysPath, sizeof(winSysPath));

	wsprintf(str, L"%s\\regsvr32.exe  ..\\serverLibraries\\ExeServerProxyStub.dll", winSysPath);
	if (MyCreateProcess(str) == 0)
	{
		trace("InitializeRegisterServer Failed");
	}
	else
	{
		trace("RegisterServer for ProxyStub Succeeded");
	}

	wsprintf(str, L"..\\serverLibraries\\ExeServer.exe RegSvr");
	if (MyCreateProcess(str) == 0)
	{
		trace("CreateProcess for ExeServer.exe failed", S_FALSE);
	}
	else
	{
		trace("Register server of EXE succeeded");
	}
}


void UninitializeRegisterServer(void)
{
	TCHAR winSysPath[MAX_PATH];
	TCHAR str[MAX_PATH];


	wsprintf(str, L"..\\serverLibraries\\ExeServer.exe UnRegSvr");
	if (MyCreateProcess(str) == 0)
	{
		trace("CreateProcess for ExeServer.exe failed", S_FALSE);
	}
	else
	{
		trace("UnRegister server for EXE succeeded");
	}

	GetSystemDirectory(winSysPath, sizeof(winSysPath));
	wsprintf(str, L"%s\\regsvr32.exe  -u ..\\serverLibraries\\ExeServerProxyStub.dll", winSysPath);
	if (MyCreateProcess(str) == 0)
	{
		trace("UnInitializeRegisterServer Failed");
	}
	else
	{
		trace("UnregisterServer for ProxyStub Succeeded");
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


STDAPI DllCallgetSine(float angle, float *result)
{
	return pITrigo->getSine(angle, result);
}

STDAPI DllCallgetCosine(float angle, float *result)
{
	return pITrigo->getCosine(angle, result);

}

STDAPI DllCallgetTangent(float angle , float* result)
{
	return pITrigo->getTangent(angle, result);
}

STDAPI DllActivateTrigoLibrary(void)
{
	HRESULT hr;

	hr = CoCreateInstance(CLSID_CTrigo, NULL, CLSCTX_LOCAL_SERVER, IID_ITrigo, (void**)&pITrigo);
	if (SUCCEEDED(hr))
	{
		trace("coCreateInstance success");
	}
	else
	{
		trace("CoCreateInstance Failed");
	}

	return S_OK;
}













