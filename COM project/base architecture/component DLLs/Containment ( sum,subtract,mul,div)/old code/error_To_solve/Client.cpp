#include <Windows.h>
#include"interface_inner.h"
#include"interface_outer.h"
#include "Util.h"
#include "RegisterAPI.h"


static inline void trace(const char* msg)
{
	Util::Trace("ClientOfClassFactory", msg, S_OK);
}
static inline void trace(const char* msg, HRESULT hr)
{
	Util::Trace("ClientOfClassFactory", msg, hr);
}

TCHAR gszWinSysDir[255];
TCHAR gstr[255];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdline, int nCmdShow)
{
	
	HWND hwnd;
	WNDCLASSEX wndclass;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyComClientApplication");

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName, TEXT("ClassfactoryClient"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	if (hwnd == NULL)
	{
		trace("CreateWindow failed");
		MessageBox(NULL, TEXT("gere"), TEXT("error"), MB_OK);
		return 1;
	}
	
	
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd , UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	ISum* pISum = NULL;
	ISubtract* pISubtract = NULL;
	IMultiply* pIMultiply = NULL;
	IDivide* pIDivide = NULL;
	int n1, n2, n3;
	TCHAR str[255];
	
	void register_server(void);
	void unregister_server(void);
	switch (iMsg)
	{
		case WM_CREATE:
			
			//trace("before");
			register_server();
			hr = CoInitialize(NULL);
			
			if(FAILED(hr))
			{
				trace("WndProc->CoInitialize", hr);
				break;
			}

		    hr = CoCreateInstance(CLSID_CSumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_IMultiply, (void**)&pIMultiply);
			if (FAILED(hr))
			{
				trace("WndProc->CoCreateInstance", hr);
				break;
			}
			//trace("after");

			n1 = 25;
			n2 = 20;
		/*	pISum->SumOfTwoNumbers(n1, n2, &n3);
			wsprintf(str, TEXT("Sum of %d and %d is  %d"), n1, n2, n3);
			MessageBox(NULL, str, TEXT("Results"), MB_OK);
			
			pISum->QueryInterface(IID_ISubtract, (void**)&pISubtract);
			pISum->Release();
			pISubtract->SubtractionOfTwoNumbers(n1, n2, &n3);
			wsprintf(str, TEXT("Subtraction of %d and %d is  %d"), n1, n2, n3);
			MessageBox(NULL, str, TEXT("Results"), MB_OK);
			
			pISubtract->QueryInterface(IID_IMultiply, (void**)&pIMultiply);*/

			pIMultiply->MultiplicationOfTwoNumbers(n1, n2, &n3);
			wsprintf(str, TEXT("Multiplication of %d and %d is  %d"), n1, n2, n3);
			MessageBox(NULL, str, TEXT("Results"), MB_OK);			
			//pISubtract->Release();
			
			pIMultiply->QueryInterface(IID_IDivide, (void**)&pIDivide);
			pIDivide->DivisionOfTwoNumbers(n1, n2, &n3);
			wsprintf(str, TEXT("Division of %d and %d is  %d"), n1, n2, n3);
			MessageBox(NULL, str, TEXT("Results"), MB_OK);
			pIMultiply->Release();
			pIDivide->Release();
			
			break;
		case WM_DESTROY:
			CoUninitialize();
			unregister_server();
			PostQuitMessage(0);
			break;
		default:
			break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}


void register_server()
{

	// Register the Server Components
	GetSystemDirectory(gszWinSysDir, 255);
//	wsprintf(gstr, TEXT("%s\\regsvr32.exe /s OuterServer.dll"), gszWinSysDir);
//	WinExec(gstr, NULL);
	wsprintf(gstr, TEXT("%s\\regsvr32.exe /s InnerServer.dll"), gszWinSysDir);
	WinExec(gstr,NULL);
}

void unregister_server()
{
	GetSystemDirectory(gszWinSysDir, 255);
	//wsprintf(gstr, TEXT("%s\\regsvr32.exe /s /u OuterServer.dll"),gszWinSysDir);
	//WinExec(gstr, NULL);
	wsprintf(gstr, TEXT("%s\\regsvr32.exe /s /u InnerServer.dll"), gszWinSysDir);
	WinExec(gstr, NULL);

}