#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "Util.h"
#include "register.h"
#include "ExeServerClient.h"



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
	typedef HRESULT(__stdcall *pfn)(float, float*);
	typedef HRESULT(__stdcall *pfn1)(void);


	TCHAR szPath[MAX_PATH];
	HMODULE hLib;
	TCHAR str[MAX_PATH];
	char astr[MAX_PATH];
	HRESULT hr;
	
	float n1, n2;

	pfn fnSine;
	pfn fnCosine;
	pfn fnTangent;
	pfn1 activateLib;

	
	switch (iMsg)
	{
	case WM_CREATE:
		wsprintf(str, L"..\\serverLibraries\\ExeServerClient.dll");

		hLib = LoadLibrary(str);
		if (hLib == NULL)
		{
			trace("LoadLibrary failed");
		}
		else
		{
			activateLib = (pfn1)GetProcAddress(hLib, "DllActivateTrigoLibrary");
			if (activateLib == 0)
			{
				trace("DllActivateTrigoLibrary failed to get");
				exit(0);
			}
			else
			{
				trace("now activating the cocreateInstance call");
				activateLib();
			}

			n1 = 30.0f;
			trace("success");
			fnSine = (pfn)GetProcAddress(hLib,"DllCallgetSine");
			if (fnSine == NULL)
			{
				trace("getProcAddressFAILED");
				DestroyWindow(hwnd);
			}
			fnSine(n1, &n2);
			sprintf(astr, "sine of %f is %f", n1, n2);
			MessageBoxA(NULL, astr,"Sine of angle", MB_OK);
		}
		break;
	case WM_DESTROY:
		FreeLibrary(hLib);
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}



