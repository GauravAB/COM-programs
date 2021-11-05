#include <Windows.h>
#include"interface.h"
#include "Util.h"
#include "RegisterAPI.h"
#include "interface_inner.h"



static inline void trace(const char* msg)
{
	Util::Trace("ClientAggregation", msg, S_OK);
}
static inline void trace(const char* msg, HRESULT hr)
{
	Util::Trace("ClientAggregation", msg, hr);
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

	hwnd = CreateWindow(szAppName, TEXT("AggregationClient"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
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
	IX* pIX = NULL;
	IY* pIY = NULL;

	//ISubtract* pISubtract = NULL;
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
			trace("creating component CA");
		    hr = CoCreateInstance(CLSID_CA, NULL, CLSCTX_INPROC_SERVER, IID_IX, (void**)&pIX);
			if (FAILED(hr))
			{
				trace("WndProc->CoCreateInstance", hr);
				break;
			}
			trace("got component now using interface IX");
			//trace("after");
			pIX->Fx();
			trace("getting pIY from pIX");
			pIX->QueryInterface(IID_IY, (void**)&pIY);
			pIX->Release();
			pIY->Fy();
			pIY->Release();
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
	wsprintf(gstr, TEXT("%s\\regsvr32.exe  /s AggregationWithoutRegFileOuter.dll"), gszWinSysDir);
	WinExec(gstr, NULL);
	wsprintf(gstr, TEXT("%s\\regsvr32.exe  /s AggregationWithoutRegFileInner.dll"), gszWinSysDir);
	WinExec(gstr,NULL);
	
}

void unregister_server()
{
	GetSystemDirectory(gszWinSysDir, 255);
	wsprintf(gstr, TEXT("%s\\regsvr32.exe /u /s AggregationWithoutRegFileInner.dll"), gszWinSysDir);
	WinExec(gstr, NULL);
	wsprintf(gstr, TEXT("%s\\regsvr32.exe /u /s AggregationWithoutRegFileOuter.dll"), gszWinSysDir);
	WinExec(gstr, NULL);
	
}