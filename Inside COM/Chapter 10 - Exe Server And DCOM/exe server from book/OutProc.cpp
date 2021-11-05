#include <Windows.h>
#include <stdlib.h>
#include "CUnknown.h"
#include "CFactory.h"



HWND g_hWndListBox = NULL;
	
BOOL InitWindow(int nCmdShow);
extern "C" LONG APIENTRY MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

extern "C" int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	//controls whether ui is shown or not
	BOOL bUI = TRUE;
	//if true dont loop
	BOOL bExit = FALSE;

	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		return 0;
	}

	//Get Thread ID;
	CFactory :: s_dwThreadID = ::GetCurrentThreadId();
	CFactory :: s_hModule = hInstance;


	//Read the command line
	char szTokens[] = "-/";

	char *szToken = strtok(lpCmdLine,szTokens);

	while(szToken != NULL)
	{
		if (_stricmp(szToken, "Regserver") == 0)
		{
			CFactory::RegisterAll();
			//we are done , so exit
			bExit = TRUE;
			bUI = FALSE;
		}
		else if (_stricmp(szToken, "UnregServer") == 0)
		{
			CFactory::UnRegisterAll();
			//we are done , so exit
			bExit = TRUE;
			bUI = FALSE;
		}
		else if (_stricmp(szToken, "Embedding") == 0)
		{
			//Dont display a window if we are embedded
			bUI = FALSE;
			break;
		}
		szToken = strtok(NULL, szTokens);
	}

	//if user started us than show UI
	if (bUI)
	{
		if (!InitWindow(nCmdShow))
		{
			//exit since we can't show UI
			bExit = TRUE;
		}
		else
		{
			::InterlockedIncrement(&CFactory::s_cServerLocks);
		}
	}

	if (!bExit)
	{
		//Register all of the class factories
		CFactory::StartFactories();

		//wait for shut down
		while (GetMessage(&msg, NULL, 0, 0))
		{
			::DispatchMessage(&msg);
			::TranslateMessage(&msg);
		}

		//Unregister the class factories
		CFactory::StopFactories();
	}

	//Uninitialize the COM LIBRARY
	CoUninitialize();
	return 0;
}


BOOL InitWindow(int nCmdShow)
{
	//create window here
	WNDCLASS wcListview;
	wcListview.style = 0;
	wcListview.lpfnWndProc = WndProc;
	wcListview.lpszClassName = "mySer verWinodowClass";
	wcListview.lpszMenuName = NULL;
	wcListview.cbClsExtra = 0;
	wcListview.cbWndExtra = 0;
	wcListview.hInstance = CFactory::s_hModule;
	wcListview.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcListview.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcListview.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);

	BOOL bResult = ::RegisterClass(&wcListview);
	if (!bResult)
	{
		return(bResult);
	}

	HWND hwndMain;

	hwndMain = CreateWindow(
		"myServerClass",
		"Component Server",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		CFactory::s_hModule,
		NULL);

	//if window failed to create than return 
	if (!hwndMain)
	{
		return FALSE;
	}

	//Making window visible and updating its client area
	::ShowWindow(hwndMain, nCmdShow);
	::UpdateWindow(hwndMain);

	return TRUE;
}


//Main window procedure

 
extern "C" LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	DWORD dwStyle;

	switch (msg)
	{
	case WM_CREATE:
		//Get size of main window
		CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;

		//Create list box for output
		g_hwndListBox = ::CreateWindow(
			"LISTBOX",
			NULL,
			WS_CHILD | WS_VISIBLE | LBS_USETABSTOPS | WS_VSCROLL | LBS_NOINTEGRALHEIGHT,
			0, 0, pcs->cs, pcs->cy,
			hwnd,
			NULL,
			CFactory::s_hModule,
			NULL);

		if (g_hwndListBox == NULL)
		{
			//Listbox not created 
			::MessageBox(NULL, "listbox not created!", NULL, MB_OK);

			return -1;
		}
		break;
	case WM_SIZE:
		::MoveWindow(g_hWndListBox, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		break;
	case WM_DESTROY:
		if (CFactory::CanUnloadNow() == S_OK)
		{
			//only post the quit message
			//no one using the program
			::PostQuitMessage(0);
		}
		break;
	case WM_CLOSE:
		::InterlockedDecrement(&CFactory::s_cServerLocks);
		//the listbox is going away
		g_hwndListBox = NULL;
		//fall through
		break;
	default:
		return(DefWindowProc(hwnd, msg, wParam, lParam));
	}
	return 0;
}
