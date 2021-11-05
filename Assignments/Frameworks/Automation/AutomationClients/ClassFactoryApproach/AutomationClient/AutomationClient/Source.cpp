#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "AutomationServer.h"

using std::cout;
using std::endl;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declaration
ICubeCuberoot* pICubeCuberoot = NULL;

void ComErrorDescriptionString(HWND, HRESULT);
void SafeInterfaceRelease(void);

//WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX	wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("ComClient");
	HRESULT hr;

	hr = CoInitialize(NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("COM library cannot be initialized \n"), TEXT("program Error"), MB_OK);
		exit(0);
	}

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

	ShowWindow(hwnd, nCmdShow);

	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CoUninitialize();
	return(msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//Variable declaration
	HRESULT hr;
	int iNum1, iNum2, iSum, iSubtract;
	TCHAR str[255];
	TCHAR winSysDir[MAX_PATH];
	TCHAR szPath[MAX_PATH];
	STARTUPINFO stInfo;
	PROCESS_INFORMATION pInfo;

	ZeroMemory(&stInfo, sizeof(stInfo));
	ZeroMemory(&pInfo, sizeof(pInfo));

	stInfo.cb = sizeof(STARTUPINFO);
	stInfo.dwFlags = STARTF_USESHOWWINDOW;
	stInfo.wShowWindow = SW_SHOWDEFAULT;

	switch (iMsg)
	{
	case WM_CREATE:
		
		GetSystemDirectory(winSysDir,sizeof(winSysDir)/sizeof(TCHAR));
		wsprintf(szPath,L"%s\\regsvr32.exe AutomationServer.dll", winSysDir);
		CreateProcess(NULL, szPath, NULL, NULL, NULL, DETACHED_PROCESS | IDLE_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo);

		hr = CoCreateInstance(CLSID_CCubeCubeRoot, NULL, CLSCTX_INPROC_SERVER, IID_ICubeCuberoot, (void**)&pICubeCuberoot);
		if (FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			DestroyWindow(hwnd);
		}
		else
		{

			//initialize arguments hardcoded
			iNum1 = 4;
			hr = pICubeCuberoot->CubeOfNumber(iNum1, &iNum2);
			wsprintf(str, L"cube of %d is %d \n", iNum1, iNum2);
			MessageBox(NULL, str, L"Result", MB_OK);
		}
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ComErrorDescriptionString(HWND hwnd, HRESULT hr)
{
	TCHAR* szErrorMessage = NULL;
	TCHAR str[255];

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&szErrorMessage, 0, NULL);
	MessageBox(NULL, szErrorMessage, TEXT("error"), MB_OK);
}
