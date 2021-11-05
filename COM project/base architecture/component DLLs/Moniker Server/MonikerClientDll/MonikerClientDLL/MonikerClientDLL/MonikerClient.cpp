#include <Windows.h>
#include "MonikerDllServer.h"
#include "MonikerClient.h"
#include "register.h"
#include "Util.h"

//globals
HMODULE ghModule;
TCHAR winSysPath[MAX_PATH];

void trace(const char *msg, HRESULT hr)
{
	Util::Trace("ClientOfMoniker", msg, hr);
}

void trace(const char* msg)
{
	Util::Trace("ClientOfMoniker", msg, S_OK);
}


BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason , LPVOID lpReserved)
{
	HRESULT hr;

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		trace("client Dll loaded");
		InitializeRegisterServer();
		hr = CoInitialize(NULL);
		if(FAILED(hr))
		{
			trace("CoInitialize FAILED");
		}
		getMonikerAndBinder();
		break;
	case DLL_PROCESS_DETACH:
		trace("client Dll Unloading");
		//Release moniker and Binder
		pIMoniker->Release();
		pIMoniker = NULL;
		pIBindCtx->Release();
		pIBindCtx = NULL;
		InitializeUnRegisterServer();
		CoUninitialize();
		break;
	default:
		break;
	}
	return TRUE;
}



void InitializeRegisterServer(void)
{
	TCHAR str[255];
	GetSystemDirectory(winSysPath, sizeof(winSysPath));

	wsprintf(str, L"%s\\regsvr32.exe -s ..\\serverLibraries\\MonikerDllServer.dll", winSysPath);
	if (MyCreateProcess(str) == 0)
	{
		trace("InitializeRegisterServer Failed");
	}
}


void InitializeUnRegisterServer(void)
{
	TCHAR str[255];
	GetSystemDirectory(winSysPath, sizeof(winSysPath));
	wsprintf(str, L"%s\\regsvr32.exe  -u -s ..\\serverLibraries\\MonikerDllServer.dll", winSysPath);
	if (MyCreateProcess(str) == 0)
	{
		//trace("UnInitializeRegisterServer Inner Failed");
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


void getMonikerAndBinder(void)
{

	//variable declarations
	ULONG uEaten;
	HRESULT hr;
	LPOLESTR szCLSID = NULL;
	wchar_t wszCLSID[255], wszTemp[255], *ptr;
	int iFirstRandomNumber;


	if (hr = CreateBindCtx(0, &pIBindCtx) != S_OK)
	{
		trace("Failed to get IBindCtx Interface pointer");
	}

	//Get String from Binary CLSID 
	StringFromCLSID(CLSID_RandomNumber, &szCLSID);
	wcscpy_s(wszTemp, szCLSID);
	ptr = wcschr(wszTemp, '{');
	//remove first '{'
	ptr = ptr + 1;
	wcscpy_s(wszTemp, ptr);

	//to remove last closing '}' from CLSID string
	wszTemp[(int)wcslen(wszTemp) - 1] = '\0';
	wsprintf(wszCLSID, L"clsid:%s", wszTemp);

	//get moniker from this CLSID 
	hr = MkParseDisplayName(pIBindCtx, wszCLSID, &uEaten, &pIMoniker);

	if (FAILED(hr))
	{
		//MessageBox(hwnd, TEXT("Failed to get IMoniker Interface pointer"), TEXT("Error"), MB_OK);
		trace("MkParseDisplayName", hr);
		pIBindCtx->Release();
		pIBindCtx = NULL;
	}

	//Bind the moniker to the named object
	hr = pIMoniker->BindToObject(pIBindCtx, NULL, IID_IRandomNumberFactory, (void**)&pIRandomNumberFactory);
	if (FAILED(hr))
	{
		trace("Failed to get custom activation - IOddNumberFactory");
		pIMoniker->Release();
		pIMoniker = NULL;
		pIBindCtx->Release();
		pIBindCtx = NULL;
	}

	//Initialize arguments hardcoded
	iFirstRandomNumber = 3;
	//call setOddNumberFactory to get first odd numbers
	hr = pIRandomNumberFactory->SetSeed(iFirstRandomNumber, &pIRandomNumber);

	if (FAILED(hr))
	{
		trace("Can Not Obtain IRandomNumber Interface");
	}

	//Releae IOddNumber Factory Interface
	pIRandomNumberFactory->Release();
	pIRandomNumberFactory = NULL;

}


STDAPI DllGetNextOperator(int Seed , int *index)
{
	pIRandomNumber->changeSeed(Seed);
	pIRandomNumber->GetNextRandomNumber(index);

	return S_OK;
}