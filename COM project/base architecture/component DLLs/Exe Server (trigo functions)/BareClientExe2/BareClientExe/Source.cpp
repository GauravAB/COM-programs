#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "Util.h"
#include "exe_server.h"
#include "register.h"
#include <cmath>

#define PI 3.14

void trace(const char*msg)
{
	Util::Trace("BARE CLIENT: ",msg,S_OK);
}

void trace(const char*msg, HRESULT hr)
{
	Util::Trace("BARE CLIENT: ", msg , hr);
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX	wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("BareClientTEST");
	HRESULT hr;

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName, TEXT("Client of COM DLL SERVER"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return(msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	typedef void(*pfn)(float, float*);

	TCHAR winSys[MAX_PATH];
	TCHAR currentPath[MAX_PATH];
	HMODULE hLib;
	TCHAR str[MAX_PATH];
	char str_p[MAX_PATH];
	pfn callsine;
	STARTUPINFO stInfo;
	PROCESS_INFORMATION pInfo;
	HRESULT hr;
	ITrigo* pITrigFunc = NULL;


	ZeroMemory(&stInfo, sizeof(stInfo));
	ZeroMemory(&pInfo, sizeof(pInfo));

	stInfo.cb = sizeof(STARTUPINFO);
	stInfo.dwFlags = STARTF_USESHOWWINDOW;
	stInfo.wShowWindow = SW_SHOWDEFAULT;

	float fangle, result;

	switch (iMsg)
	{
	case WM_CREATE:

		//GetSystemDirectory(winSys, MAX_PATH);		
		GetCurrentDirectory(MAX_PATH, currentPath);
		GetSystemDirectory(winSys,MAX_PATH);

		wsprintf(str, L"%s\\regsvr32.exe  ExeServerProxyStub.dll",winSys);
	
		if (0 == CreateProcess(NULL, str, NULL, NULL, NULL, NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo))
		{
			trace("CreateProcess failed", S_FALSE);
		}
		else
		{
			WaitForInputIdle(pInfo.hProcess, INFINITE);
		}

		wsprintf(str, L"ExeServer.exe RegSvr");
		if (0 == CreateProcess(NULL, str, NULL, NULL, NULL, NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo))
		{
			trace("CreateProcess for ExeServer.exe failed",S_FALSE);
		}
		else
		{
			WaitForInputIdle(pInfo.hProcess, INFINITE);
		}

		CoInitialize(NULL);
	
		hr = CoCreateInstance(CLSID_CTrigo, NULL, CLSCTX_LOCAL_SERVER,IID_ITrigo,(void**)&pITrigFunc);
		
		if (FAILED(hr))
		{
			trace("CoCreateInstance failed",hr);
		}
		else
		{
			trace("happy:");
		}
	
		fangle = PI/2;
		
		pITrigFunc->getSine(fangle, &result);
		result = (result * 180) / PI;
		sprintf(str_p, "sine of %g is %g", fangle, result);
		MessageBoxA(NULL, str_p, "SineofAngle", MB_OK);
		
		/*
		GetCurrentDirectory(MAX_PATH, szPath);
		wsprintf(str, L"ClientToExeServer.dll");
		hLib = LoadLibrary(str);

		if (hLib == NULL)
		{
			trace("LoadLibrary failed");
		}
		else
		{
			trace("load library success");
		}
		*/
		
		break;
	case WM_DESTROY:

		wsprintf(str, L"ExeServer.exe UnRegSvr");	
		if (0 == CreateProcess(NULL, str, NULL, NULL, NULL, NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo))
		{
			trace("CreateProcess ExeServer.exe Unreg failed", S_FALSE);
		}
		else
		{
			WaitForInputIdle(pInfo.hProcess, INFINITE);
		}

		GetSystemDirectory(winSys, MAX_PATH);

		wsprintf(str, L"%s\\regsvr32.exe -u  ExeServerProxyStub.dll", winSys);
		if (0 == CreateProcess(NULL, str, NULL, NULL, NULL, NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo))
		{
			trace("CreateProcess failed", S_FALSE);
		}
		else
		{
			WaitForInputIdle(pInfo.hProcess, INFINITE);
		}

		CoUninitialize();
		PostQuitMessage(0);

		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
