#include <Windows.h>
#define WIN_WIDTH 1300
#define WIN_HEIGHT 800

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK dlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	HWND hwnd;
	WNDCLASSEX wndclass;
	MSG msg;
	TCHAR szAppName[] = L"MathServerClient";


	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wndclass);

	int maxWidth = GetSystemMetrics(SM_CXMAXIMIZED);
	int maxHeight = GetSystemMetrics(SM_CXMINIMIZED);

	hwnd = CreateWindowEx(WS_EX_APPWINDOW,szAppName, L"MyMathServer", WS_OVERLAPPEDWINDOW, (maxWidth/2)-(WIN_WIDTH/2),(maxHeight/2)-90, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return ((int)msg.wParam);
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	CHAR header[] = "I  AM  A  MATH  SERVER";
	HFONT myHugeFont;
	static bool activateServer;
	HBRUSH hBrush;
	static HINSTANCE hInstance;

	myHugeFont = CreateFont(50, 30, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DECORATIVE, NULL);


	switch (iMsg)
	{
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			if (activateServer == false)
			{
				SetTextColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, TRANSPARENT);
				SelectObject(hdc, myHugeFont);
				TextOutA(hdc, 250, 200, header, sizeof(header));
			}
			else
			{
				hBrush = CreateSolidBrush(RGB(0, 0, 0));
				SelectObject(hdc, hBrush);
				Rectangle(hdc, 100, 150, 1180, 350);
			}

			EndPaint(hwnd, &ps);
			break;
		case WM_KEYDOWN:
			
			switch (wParam)
			{
				case VK_ESCAPE:
					DestroyWindow(hwnd);
					break;
				case VK_SPACE:
					if (activateServer == false)
					{
						activateServer = true;
					}
					InvalidateRect(hwnd, NULL, TRUE);
					DialogBox(hInstance, TEXT("DIALOGBOX"), hwnd, dlgProc);
					break;
			}
			break;
		case WM_CREATE:
			hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
			activateServer = false;
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			}
			break;
	}

	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}
