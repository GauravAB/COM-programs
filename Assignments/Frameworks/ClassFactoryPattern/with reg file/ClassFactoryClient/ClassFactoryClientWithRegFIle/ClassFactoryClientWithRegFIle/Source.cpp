#include <windows.h>
#include "Header.h"
#include <stdlib.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#define WINWIDTH 800
#define WINHEIGHT 600

//pointers to the interfaces
ISum *pISum = NULL;
ISubtract *pISubtract = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("ComClient");
	HRESULT hr;

	//initialize COM library
	hr = CoInitialize(NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("COM library cannot be initialized"), TEXT("error message"), MB_OK);
		exit(0);
	}

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName, TEXT("my classfactory client window"), WS_OVERLAPPEDWINDOW, 0, 0, WINWIDTH, WINHEIGHT, 0, 0, hInstance, 0);

	if (!hwnd)
	{
		MessageBox(NULL, TEXT("failed to create window"), TEXT("error message"), MB_OK);
		exit(1);
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, hwnd, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CoUninitialize();

	return((int)msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	void SafeInterfaceRelease(void);
	HRESULT hr;
	int iNum1, iNum2, iSum,iSub;
	TCHAR str[255];

	switch (iMsg)
	{
	case WM_CREATE:
		hr = CoCreateInstance(CLSID_SumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void**)&pISum);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("failed to create coClass instance"), TEXT("error message"), MB_OK);
			DestroyWindow(hwnd);
		}
		iNum1 = 55;
		iNum2 = 45;
		pISum->SumOfTwoIntegers(iNum1, iNum2, &iSum);
		wsprintf(str, TEXT("Sum of %d And %d = %d"),iNum1,iNum2,iSum);
		MessageBox(NULL, str, TEXT("Result"), MB_OK);

		hr = pISum->QueryInterface(IID_ISubtract, (void**)&pISubtract);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("ISubtract Interface cannot be obtained"), TEXT("error"), MB_OK);
			DestroyWindow(hwnd);
		}
		pISubtract->SubtractionOfTwoIntegers(iNum1, iNum2,&iSub);
		pISum->Release();
		pISum = NULL;

		wsprintf(str, TEXT("Subtraction of %d And %d = %d"), iNum1, iNum2, iSub);
		MessageBox(NULL, str, TEXT("Result"), MB_OK);

		pISubtract->Release();
		pISubtract = NULL;
		
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void SafeInterfaceRelease(void)
{
	if (pISum)
	{
		pISum->Release();
		pISum = NULL;
	}

	if (pISubtract)
	{
		pISubtract->Release();
		pISubtract = NULL;
	}
}








