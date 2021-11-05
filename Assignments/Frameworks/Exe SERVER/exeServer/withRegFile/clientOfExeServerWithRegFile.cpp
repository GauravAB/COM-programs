#include <Windows.h>
#include "exeServerWithRegFile.h"
#include <process.h>

LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR AppName[] = TEXT("client");

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpszClassName = AppName;
	wndclass.lpszMenuName = NULL;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = wndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	RegisterClassEx(&wndclass);

	//creating window now

	hwnd = CreateWindow(AppName, TEXT("client to exe server"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);


	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//variables required
	ISum* pISum = NULL;
	ISubtract* pISubtract = NULL;

	HRESULT hr;
	int error, n1, n2, n3;
	TCHAR szWinSysDir[255];
	static TCHAR szPath[_MAX_PATH];
	TCHAR str[255];
	void* pMsgBuf;

	switch (iMsg)
	{
		case WM_CREATE:
			
			//Initialize COM library
			hr = CoInitialize(NULL);

			if (FAILED(hr))
			{
				MessageBox(hwnd, TEXT("COM library cannot be initialized"), TEXT("COM error"), MB_OK);
				DestroyWindow(hwnd);
				exit(0);
			}

			//get ISum Interface
			hr = CoCreateInstance(CLSID_SumSubtract, NULL, CLSCTX_LOCAL_SERVER, IID_ISum, (void**)&pISum);

			if (FAILED(hr))
			{
				FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,NULL,hr,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR)&pMsgBuf,0,NULL);
				MessageBox(NULL, (LPSTR)pMsgBuf, TEXT("COM error"), MB_OK);
				DestroyWindow(hwnd);
				exit(0);
			}

			n1 = 25;
			n2 = 5;
			pISum->SumOfTwoIntegers(n1, n2, &n3);
			wsprintf(str, TEXT("sum of %d and %d is %d"), n1, n2, n3);
			MessageBox(hwnd, str, TEXT("SUM"), MB_OK);
			hr = pISum->QueryInterface(IID_ISubtract, (void**)&pISubtract);
			pISubtract->SubtractionOfTwoIntegers(n1, n2, &n3);
			wsprintf(str, TEXT("Subtraction of %d and %d is %d"), n1, n2, n3);
			MessageBox(hwnd, str, TEXT("Substraction"), MB_OK);
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			CoUninitialize();
			
			PostQuitMessage(0);
			break;
		default:
			break;
	}


	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}
