#import "dotNetServer.tlb" no_namespace, raw_interface_only
#include "doNetServer.tlh"

#include <Windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTNACE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR AppName[] = TEXT("ComClient");
	HRESULT hr;

	//COM Initialization
	hr = CoInitialize(NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("COM Library Can not Be initialized.\nPRogram Will now Exit."), TEXT("Program Error"), MB_OK);
		exit(0);
	}

	//WNDCLASEX initialization
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpszClassName = AppName;
	wndclass.lpszMenuName = NULL;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);


	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(AppName, L"Client of dotNetDLLServer", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CoUninitialize();

	return(int(msg.wParam));
}

//Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	void ComErrorDescriptionString(HWND, HRESULT);
	HRESULT hr;
	int iNum1, iNum2, iMultiply;
	TCHAR str[255];

	switch (iMsg)
	{
	case WM_CREATE:
		CLSID clsidMath;
		iNum1 = 10;
		iNum2 = 20;
		long iMultiply = 0;
		_Math* app;

		hr = CLSIDFromProgID(L"ManagedServerForInterop.dotNetServer", &clsidMath);

		if (FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			DestroyWindow(hwnd);
		}

		hr = CoCreateInstance(clsidMath, NULL, CLSCTX_INPROC_SERVER, __uuidof(_Math), (void**)&app);
		if (FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			DestroyWindow(hwnd);
		}
		hr = app->MultiplicationOfTwoIntegers(10, 20, &iMultiply);

		if (FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			DestroyWindow(hwnd);
		}
		wsprintf(str, TEXT("Multiplication of %d and %d is %d "), iNum1, iNum2, iMultiply);
		MessageBox(hwnd, str, TEXT("MultiplicationOfTwoIntegers"), MB_OK);
		break;
		/*__MathPtr pFoo(__uuidof(Math));*/
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	DefWindowProc(hwnd, iMsg, wParam, lParam);
}


void ComErrorDescriptionString(HWND hwnd, HRESULT hr)
{
	//variable declaration
	TCHAR *szErrorMessage = NULL;
	TCHAR str[255];

	if (FACILITY_WINDOWS == HRESULT_FACILITY(hr))
	{
		hr = HRESULT_CODE(hr);
	}

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&szErrorMessage, 0, NULL) != 0)
	{
		swprintf_s(str, TEXT("%s"), szErrorMessage);
		LocalFree(szErrorMessage);
	}
	else
	{
		swprintf_s(str, TEXT("could not find a description for error #%#x.]\n"), hr);
	}

	MessageBox(hwnd, str, TEXT("COM Error"), MB_OK);
}


