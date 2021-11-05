#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "Util.h"
#include "ExeServerWithoutRegFile.h"
#include "register.h"
#include "ContainmentClient.h"

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
	typedef HRESULT (__stdcall *pfn)(int, int, int*);

	TCHAR szPath[MAX_PATH];
	HMODULE hLib;
	TCHAR str[64];

	HRESULT hr;
	
	int n1; int n2;int n3;

	pfn fnSum;
	pfn fnSub;
	pfn fnMul;
	pfn fnDiv;

	switch (iMsg)
	{
	case WM_CREATE:

		GetCurrentDirectory(MAX_PATH, szPath);
		wsprintf(str, L"ContainmentClient.dll");
		hLib = LoadLibrary(str);
		if (hLib == NULL)
		{
			trace("LoadLibrary failed");
		}
		else
		{
			
			fnSum = (pfn)GetProcAddress(hLib, "DllCallSumOfTwoIntegers");
			if (fnSum == NULL)
			{
				trace("GetProcAddress Failed");
			}
			else
			{
				trace("Success GetProcAddress");
				n1 = 20;
				n2 = 100;
				fnSum(n1, n2, &n3);
				wsprintf(str, L"sum of %d and %d is %d", n1, n2, n3);
				MessageBox(NULL, str, L"SumOfTwoIntegers", MB_OK);
			}

			fnSub = (pfn)GetProcAddress(hLib, "DllCallSubtractionOfTwoIntegers");
			if (fnSub == NULL)
			{
				trace("GetProc failed for DllCallSubtractionOfTwoIntegers");
			}
			else
			{
				trace("Success GetProcAddress");
				n1 = 100;
				n2 = 20;
				fnSub(n1, n2, &n3);
				wsprintf(str, L"subtraction of %d and %d is %d", n1, n2, n3);
				MessageBox(NULL, str, L"SubtractionOfTwoIntegers", MB_OK);
			}
			
			fnMul = (pfn)GetProcAddress(hLib,"DllCallMultiplicationOfTwoIntegers");
			if (fnMul == NULL)
			{
				trace("GetProc failed for DllCallMultiplicationOfTwoIntegers");
			}
			else
			{
				trace("Success GetProcAddress");
				n1 = 20;
				n2 = 20;
				fnMul(n1, n2, &n3);
				wsprintf(str, L"multiplication of %d and %d is %d", n1, n2, n3);
				MessageBox(NULL, str, L"MultiplicationOfTwoIntegers", MB_OK);
			}
			fnDiv = (pfn)GetProcAddress(hLib, "DllCallDivisionOfTwoIntegers");
			if (fnDiv == NULL)
			{
				trace("GetProc failed for DllCallDivisionOfTwoIntegers");
			}
			else
			{
				trace("Success GetProcAddress");
				n1 = 40;
				n2 = 2;
				fnDiv(n1, n2, &n3);
				wsprintf(str, L"division of %d and %d is %d", n1, n2, n3);
				MessageBox(NULL, str, L"divisionOfTwoIntegers", MB_OK);
			}

		}
		/*
		wsprintf(str, L"ClassFactoryClient.dll");
		hLib = LoadLibrary(str);

		if (hLib == NULL)
		{
			trace("LoadLibrary failed");
		}
		else
		{
			trace("load library success");
			fnSum = (pfn)GetProcAddress(hLib,"DllCallSumOfTwoIntegers");
			if (fnSum == NULL)
			{
				trace("GetProcAddress Failed");
			}
			else
			{
				trace("Success GetProcAddress");
				n1 = 20;
				n2 = 100;
				fnSum(n1, n2, &n3);
				wsprintf(str, L"sum of %d and %d is %d", n1, n2, n3);
				MessageBox(NULL, str, L"SumOfTwoIntegers", MB_OK);

				fnSub = (pfn)GetProcAddress(hLib, "DllCallSubtractionOfTwoIntegers");
				if (fnSub == NULL)
				{
					trace("GetProc failed for DllCallSubtractionOfTwoIntegers");
				}
				else
				{
					trace("Success GetProcAddress");
					n1 = 100;
					n2 = 20;
					fnSub(n1, n2, &n3);
					wsprintf(str, L"subtraction of %d and %d is %d", n1, n2, n3);
					MessageBox(NULL, str, L"SubtractionOfTwoIntegers", MB_OK);
				}
			}
		}
		*/
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
