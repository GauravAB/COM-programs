
#include "Util.h"
#include <cstdlib>
#include "CFactory.h"
#include "Server.h"

static inline void trace(const char* msg)
{
	Util::Trace("Server:", msg, S_OK);
}

static inline void trace(const char* msg, HRESULT hr)
{
	Util::Trace("Server:", msg, hr);
}



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wndclass;
	TCHAR szAppName[] = TEXT("myExeServer");
	BOOL bCOMServerCalls = FALSE;
	BOOL bShowUI = TRUE;
	BOOL bExit = FALSE;
	char szTokenizer[] = "-/";
	char *szToken = NULL;
	HRESULT hr;

	hr = CoInitialize(NULL);
	trace("CoInitialize", hr);
	if (FAILED(hr))
	{
		return 0;
	}

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);
	hwnd = CreateWindow(szAppName, TEXT("myCOMExeServer"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	
	szToken = strtok(szCmdLine, szTokenizer);

	while (szToken != NULL)
	{
		if (_strcmpi(szToken, "RegisterServer") == 0)
		{	
			//call CFactory Register Server and register all interfaces
			bShowUI = FALSE;
			bExit = TRUE;
		}
		else if (_stricmp(szToken, "UnRegisterServer") == 0)
		{
			//call CFactory UnRegister Server and remove all interfaces
			bShowUI = FALSE;
			bExit = TRUE;
		}
		else if(_stricmp(szToken,"Embedding") == 0)
		{
			//do not show UI but continue message loop
			bShowUI = FALSE;
			bCOMServerCalls = TRUE;
			break;
		}
		szToken = strtok(NULL, szTokenizer);
	}



	if (hwnd == NULL)
	{
		MessageBox(NULL, TEXT("hwnd creation failed"), TEXT("error"), MB_OK);
		DestroyWindow(hwnd);
		exit(1);
	}

	if (bCOMServerCalls == TRUE)
	{
		//global increment for this server
		InterlockedIncrement(&CFactory::s_cServerLocks);
		hr = CFactory::StartFactories();
		trace("StartFactories:", hr);
		ShowWindow(hwnd, SW_HIDE);
	}
	else
	{
		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);
	}
	

	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (bCOMServerCalls == TRUE)
	{
		CFactory::StopFactories();
	}

	CoUninitialize();
	return((int)msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd , UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}







