#include <Windows.h>	
#include <stdlib.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

ISum *pISum = NULL;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow)
{
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppname[] = TEXT("myclassfactoryclient");

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpszClassName = szAppname;
	wndclass.lpszMenuName = NULL;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppname, TEXT("myWindow"), WS_OVERLAPPEDWINDOW, 0, 0, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, hInstance, NULL);
	
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
	void safeInterfaceRelease(void);
	HRESULT hr;
	int iNum1, iNum2, iRes;

	TCHAR str[255];



		
	switch (iMsg)
	{
		case WM_CREATE:
			hr = CoCreateInstance(CLSID_SumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void**)&pISum);
			if (FAILED(hr))
			{
				MessageBox(hwnd, TEXT("failed to cocreateInstance"), TEXT("error"), MB_OK);
				DestroyWindow(hwnd);
			}
			iNum1 = 55;
			iNum2 = 45;

			pISum->SumOfTwoIntegers(iNum1, iNum2, &iRes);

			break;
		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_ESCAPE:
					DestroyWindow(hwnd);
					break;
				default:
					break;
			}
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
		pISubtract = NULL;
	}
}





















