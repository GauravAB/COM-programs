#include <windows.h>
#include <stdio.h>
#include "MonikerDllServer.h"
#include "Util.h"

void trace(const char* msg, HRESULT hr)
{
	Util::Trace("MonikerClient", msg, hr);
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//global variable declarations
IOddNumber *pIOddNumber = NULL;
IOddNumberFactory *pIOddNumberFactory = NULL;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = L"ComClient";
	HRESULT hr;


	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		trace("CoInitialize failed", hr);
	}
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName, L"ComClientforMoniker", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
			
	CoUninitialize();
	return msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	void safeInterfaceRelease(void);

	//variable declarations
	IBindCtx *pIBindCtx = NULL;
	IMoniker *pIMoniker = NULL;
	ULONG uEaten;
	HRESULT hr;
	LPOLESTR szCLSID = NULL;
	wchar_t wszCLSID[255], wszTemp[255], *ptr;
	int iFirstOddNumber, iNextOddNumber;
	TCHAR str[255];	
	TCHAR winSysPath[MAX_PATH];
	STARTUPINFO stInfo;
	PROCESS_INFORMATION pInfo;

	ZeroMemory(&stInfo, sizeof(STARTUPINFO));
	ZeroMemory(&pInfo, sizeof(PROCESS_INFORMATION));


	switch (iMsg)
	{
	case WM_CREATE:

		GetSystemDirectory(winSysPath, MAX_PATH);
		wsprintf(str, L"%s\\Regsvr32.exe MonikerDllServer.dll", winSysPath);
		if (0 == CreateProcess(NULL, str, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo))
		{
			MessageBox(NULL, L"createProcess to regsvr failed",L"ClientOfMonikerServer: CreateProcess",MB_OK);
		}


		if (hr = CreateBindCtx(0, &pIBindCtx) != S_OK)
		{
			MessageBox(hwnd, TEXT("Failed to get IBindCtx Interface pointer"), TEXT("error"), MB_OK);
			DestroyWindow
			(hwnd);
		}

		//Get String from Binary CLSID 
		StringFromCLSID(CLSID_OddNumber, &szCLSID);
		wcscpy_s(wszTemp, szCLSID);
		ptr = wcschr(wszTemp, '{');
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
			DestroyWindow(hwnd);
		}
		//Bind the moniker to the named object
		hr = pIMoniker->BindToObject(pIBindCtx, NULL, IID_IOddNumberFactory, (void**)&pIOddNumberFactory);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Failed to get custom activation - IOddNumberFactory"), L"error", MB_OK);
			pIMoniker->Release();
			pIMoniker = NULL;
			pIBindCtx->Release();
			pIBindCtx = NULL;

			DestroyWindow(hwnd);
		}

		//Relase moniker and Binder
		pIMoniker->Release();
		pIMoniker = NULL;
		pIBindCtx->Release();
		pIBindCtx = NULL;

		//Initialize arguments hardcoded
		iFirstOddNumber = 57;
		//call setOddNumberFactory to get first odd numbers
		hr = pIOddNumberFactory->SetFirstOddNumber(iFirstOddNumber, &pIOddNumber);

		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Can Not Obtain IOddNumber Interface"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		//Releae IOddNumber Factory Interface
		pIOddNumberFactory->Release();
		pIOddNumberFactory = NULL;

		//call GetNextOddNumber() of IOddNumber to get the next odd number of the first odd number
		pIOddNumber->GetNextOddNumber(&iNextOddNumber);

		//Release IOddNumber Interface
		pIOddNumber->Release();
		pIOddNumber = NULL;

		//show the results
		wsprintf(str, TEXT("The next odd number from %2d is %2d"), iFirstOddNumber, iNextOddNumber);
		MessageBox(hwnd,str, TEXT("Result"), MB_OK | MB_TOPMOST);
		DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			break;
	}


	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}



void safeInterfaceRelease(void)
{
	if (pIOddNumber)
	{
		pIOddNumber->Release();
		pIOddNumber = NULL;
	}

	if (pIOddNumberFactory)
	{
		pIOddNumberFactory->Release();
		pIOddNumberFactory = NULL;
	}
}






