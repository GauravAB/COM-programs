#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "Util.h"
#include "ExeServerWithoutRegFile.h"
#include "register.h"

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

	TCHAR szPath[MAX_PATH];
	HMODULE hLib;
	TCHAR str[64];
	pfn callsine;
	STARTUPINFO stInfo;
	PROCESS_INFORMATION pInfo;
	HRESULT hr;
	ITrigoFunctions* pITrigFunc = NULL;


	ZeroMemory(&stInfo, sizeof(stInfo));
	ZeroMemory(&pInfo, sizeof(pInfo));

	stInfo.cb = sizeof(STARTUPINFO);
	stInfo.dwFlags = STARTF_USESHOWWINDOW;
	stInfo.wShowWindow = SW_SHOWDEFAULT;


	switch (iMsg)
	{
	case WM_CREATE:

		/*
		hr = CoCreateInstance(CLSID_CTrigoFunctions, NULL, CLSCTX_LOCAL_SERVER | REGCLS_MULTIPLEUSE ,IID_ITrigoFunctions,(void**)&pITrigFunc);
		
		if (FAILED(hr))
		{
			trace("sad");
		}
		else
		{
			trace("happy:");
		}
		*/

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
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
