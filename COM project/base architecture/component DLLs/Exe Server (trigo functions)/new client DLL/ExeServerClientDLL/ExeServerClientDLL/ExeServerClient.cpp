#include <Windows.h>
#include "Interface_exe_server.h"
#include "Util.h"
#include "ExeServerClient.h"

void trace(const char *msg)
{
	Util::Trace("ExeServerClientDLL: ", msg,S_OK);
}

void trace(const char*msg, HRESULT hr)
{
	Util::Trace("ExeServerClientDLL: ", msg, hr);
}

ITrigo *pITrigo = NULL;

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{


	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
			InitializeRegisterServer();
			InitializeComponent();
			break;
		case DLL_PROCESS_DETACH:
			UninitializeComponent();
			InitializeUnregisterServer();
			break;
		default:
			break;
	}

	return TRUE;
}

void InitializeRegisterServer(void)
{
	HRESULT hr;
	STARTUPINFO stInfo;
	PROCESS_INFORMATION pInfo;
	TCHAR szPath[MAX_PATH];
	TCHAR winSysPath[MAX_PATH];
	ZeroMemory(&stInfo, sizeof(STARTUPINFO));
	ZeroMemory(&pInfo, sizeof(PROCESS_INFORMATION));

	stInfo.cb = sizeof(STARTUPINFO);

	GetSystemDirectory(winSysPath,MAX_PATH);
	wsprintf(szPath, L"%s\\regsvr32.exe ..\\serverLibraries\\ExeServerProxyStub.dll",winSysPath);
	if (0 == CreateProcess(NULL, szPath, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo))
	{
		trace("CreateProcess Regsvr32 failed");
	}

	/*
	wsprintf(szPath, L"..\\serverLibraries\\ExeServer.exe RegSvr");
	if (0 == CreateProcess(NULL, szPath, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo))
	{
		trace("CreateProcess ExeServer RegSvr failed");
	}
	*/
}


void InitializeUnregisterServer(void)
{
	HRESULT hr;
	STARTUPINFO stInfo;
	PROCESS_INFORMATION pInfo;
	TCHAR szPath[MAX_PATH];
	TCHAR winSysPath[MAX_PATH];
	ZeroMemory(&stInfo, sizeof(STARTUPINFO));
	ZeroMemory(&pInfo, sizeof(PROCESS_INFORMATION));

	stInfo.cb = sizeof(STARTUPINFO);

	GetSystemDirectory(winSysPath, sizeof(winSysPath));
	wsprintf(szPath, L"%s\\regsvr32.exe -u ..\\serverLibraries\\ExeServerProxyStub.dll",winSysPath);
	if (0 == CreateProcess(NULL, szPath, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo))
	{
		trace("CreateProcess Regsvr32 -u failed");
	}

	wsprintf(szPath, L"..\\serverLibraries\\ExeServer.exe UnRegSvr");
	if (0 == CreateProcess(NULL, szPath, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo))
	{
		trace("CreateProcess ExeServer UnRegSvr failed");
	}
}


void InitializeComponent(void)
{
	HRESULT hr;
	float n1, n2;
	char str[255];

	CoInitialize(NULL);
	hr = CoCreateInstance(CLSID_CTrigo, NULL, CLSCTX_LOCAL_SERVER | CLSCTX_INPROC_SERVER, IID_ITrigo, (void**)&pITrigo);
	trace("step after coCreate");
	if (FAILED(hr))
	{
		trace("CoCreateInstance failed", hr);
	}
	else
	{
		trace("testing");
		n1 = 30.0f;
		pITrigo->getSine(n1, &n2);
		sprintf(str, "sine of %f is %f", n1, n2);
		MessageBoxA(NULL, str,"Result of Sine from DLL", MB_OK);
	}
}

void UninitializeComponent(void)
{
	if (pITrigo)
	{
		pITrigo->Release();
	}
	CoUninitialize();
}


STDAPI DllCallgetSine(float angle, float *result)
{
	return pITrigo->getSine(angle, result);
}

STDAPI DllCallgetCosine(float angle, float *result)
{
	return  pITrigo->getCosine(angle, result);
}

STDAPI DllCallgetTangent(float angle, float *result)
{
	return pITrigo->getTangent(angle, result);
}




