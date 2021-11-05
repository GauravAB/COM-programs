#include <Windows.h>
#include <stdio.h>
#include "AutomationServer.h"
#include "Util.h"

void trace(const char *msg)
{
	Util::Trace("Automation IDispatch Client:: ", msg, S_OK);
}


void trace(const char *msg, HRESULT hr)
{
	Util::Trace("IDispatch Client", msg, hr);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declaration
ICubeCuberoot* pICubeCuberoot = NULL;
TCHAR winSysDir[MAX_PATH];
TCHAR szPath[MAX_PATH];

STARTUPINFO stInfo;
PROCESS_INFORMATION pInfo;

//WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX	wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("ComClient");
	HRESULT hr;

	stInfo.cb = sizeof(STARTUPINFO);
	stInfo.dwFlags = STARTF_USESHOWWINDOW;
	stInfo.wShowWindow = SW_SHOWDEFAULT;

	GetSystemDirectory(winSysDir, sizeof(winSysDir) / sizeof(TCHAR));
	wsprintf(szPath, L"%s\\regsvr32.exe AutomationServer.dll", winSysDir);

	if (0 == CreateProcess(NULL, szPath, NULL, NULL, NULL, HIGH_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo))
	{
		trace("CreateProcess AutomationServer Registry failed");
	}
	else
	{
		WaitForInputIdle(pInfo.hProcess, INFINITE);
	}
	wsprintf(szPath, L"%s\\regsvr32.exe AutomationProxyStub.dll", winSysDir);
	if (0 == CreateProcess(NULL, szPath, NULL, NULL, NULL, HIGH_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo))
	{
		trace("CreateProcess AutomationProxyStub Registry failed");
	}
	else
	{
		WaitForInputIdle(pInfo.hProcess, INFINITE);
	}
	
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
	TCHAR str[255];
	
	IDispatch *pIDispatch = NULL;
	DISPID dispid;
	OLECHAR* szFunctionName1 = L"CubeOfNumber";
	OLECHAR* szFunctionName2 = L"CubeRootOfNumber";
	VARIANT vArg[2] ,vRet;
	DISPPARAMS param = { vArg,0,2,NULL };
	int n1, n2;
	
	ZeroMemory(&stInfo, sizeof(stInfo));
	ZeroMemory(&pInfo, sizeof(pInfo));

	switch (iMsg)
	{
	case WM_CREATE:
		
		hr = CoCreateInstance(CLSID_CCubeCubeRoot, NULL, CLSCTX_INPROC_SERVER, IID_ICubeCuberoot, (void**)&pIDispatch);
		if (FAILED(hr))
		{
			trace("CoCreateInstance failed", hr);
			DestroyWindow(hwnd);
		}
		else
		{

			trace("CoCreateInstance Success");
			//initialize arguments hardcoded
			n1 = 4;
			n2 = 125;

			//as DISPARAMS rgvarg member receives parameters in reverse order
			VariantInit(vArg);
			vArg[0].vt = VT_INT;
			vArg[0].intVal = n1;
			param.cArgs = 1;
			param.cNamedArgs = 0;
			param.rgdispidNamedArgs = NULL;
			//reverse order parameters
			param.rgvarg = vArg;
			//return value

			trace("trying Variant Init");
			VariantInit(&vRet);

			trace("trying GetIDsOfNames");
			//code to get the cube of a number
			hr = pIDispatch->GetIDsOfNames(IID_NULL, &szFunctionName1, 1, GetUserDefaultLCID(), &dispid);
			
			if (FAILED(hr))
			{
				trace("pIDspatch GetIDSOfNames", hr);
				DestroyWindow(hwnd);
			}
			else
			{
				trace("GetIDsOfNames Success");
			}
			
			hr = pIDispatch->Invoke(dispid, IID_NULL, GetUserDefaultLCID(), DISPATCH_METHOD, &param, &vRet, NULL, NULL);

			if (FAILED(hr))
			{
				trace("pIDispatch->Invoke failed", hr);
				DestroyWindow(hwnd);
			}
			else
			{
				wsprintf(str, L"cube of %d is %d \n", n1,vRet.lVal);
				MessageBox(NULL, str, L"CubeOfNumber", MB_OK);
			}

			//code to get the cuberoot of a number
			vArg[0].intVal = n2;
			hr = pIDispatch->GetIDsOfNames(IID_NULL, &szFunctionName2, 1, GetUserDefaultLCID(), &dispid);
			if (FAILED(hr))
			{
				trace("pIDspatch GetIDSOfNames", hr);
				DestroyWindow(hwnd);
			}
			else
			{
				trace("GetIDsOfNames Success");
			}

			hr = pIDispatch->Invoke(dispid, IID_NULL, GetUserDefaultLCID(), DISPATCH_METHOD, &param, &vRet, NULL, NULL);

			if (FAILED(hr))
			{
				trace("pIDispatch->Invoke failed", hr);
				DestroyWindow(hwnd);
			}
			else
			{
				wsprintf(str, L"cuberoot of %d is %d \n", n2, vRet.lVal);
				MessageBox(NULL, str, L"CubeOfNumber", MB_OK);
			}



			//clean up
			VariantClear(vArg);
			VariantClear(&vRet);
			pIDispatch->Release();
			pIDispatch = NULL;
		}
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		CoUninitialize();
		GetSystemDirectory(winSysDir, sizeof(winSysDir) / sizeof(TCHAR));
		wsprintf(szPath, L"%s\\regsvr32.exe -u AutomationServer.dll", winSysDir);
		CreateProcess(NULL, szPath, NULL, NULL, NULL, DETACHED_PROCESS | IDLE_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo);
		wsprintf(szPath, L"%s\\regsvr32.exe -u AutomationProxyStub.dll", winSysDir);
		CreateProcess(NULL, szPath, NULL, NULL, NULL, DETACHED_PROCESS | IDLE_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo);

		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

