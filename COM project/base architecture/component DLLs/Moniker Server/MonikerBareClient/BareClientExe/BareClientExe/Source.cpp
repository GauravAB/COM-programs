#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "Util.h"
#include "register.h"
#include "MonikerClient.h"


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

	typedef HRESULT (__stdcall *pfn)(int,int*);
	TCHAR szPath[MAX_PATH];
	HMODULE hLib;
	TCHAR str[64];

	HRESULT hr;
	
	int n1; int n2;

	pfn fnNextNumber;

	switch (iMsg)
	{
	case WM_CREATE:

		GetCurrentDirectory(MAX_PATH, szPath);
		wsprintf(str, L"..\\serverLibraries\\MonikerClient.dll");
		hLib = LoadLibrary(str);

		if (hLib == NULL)
		{
			trace("LoadLibrary failed");
		}
		else
		{
			trace("load library success");
			fnNextNumber = (pfn)GetProcAddress(hLib,"DllGetNextOperator");
			if (fnNextNumber == NULL)
			{
				trace("GetProcAddress Failed");
			}
			else
			{
				trace("Success GetProcAddress");
				n1 = 2;
				fnNextNumber(n1, &n2);
				wsprintf(str, L"next Operator is %2d", n2);
				MessageBox(NULL, str, L"nextNumber", MB_OK);

				fnNextNumber(n2, &n2);
				wsprintf(str, L"next Operator is %2d", n2);
				MessageBox(NULL, str, L"nextNumber", MB_OK);

			}
		}
		
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
