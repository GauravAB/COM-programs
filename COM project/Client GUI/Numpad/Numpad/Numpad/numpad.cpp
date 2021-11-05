#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	HWND hwnd;
	WNDCLASSEX wndclass;
	MSG msg;
	TCHAR szAppName[] = L"Numpad Application";


	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = DLGWINDOWEXTRA;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wndclass);

	//hwnd = CreateWindow(szAppName,L"MyNumPad",WS_OVERLAPPEDWINDOW,0,0,)

	hwnd = CreateDialog(hInstance, szAppName, 0, NULL);

	ShowWindow(hwnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void ShowNumber(HWND hwnd, UINT iNumber)
{
	TCHAR szBuffer[20];

	wsprintf(szBuffer, L"%X", iNumber);
	SetDlgItemText(hwnd, VK_ESCAPE, szBuffer);
}

DWORD calculator(UINT iFirstNum, int iOperation, UINT iNum)
{
	switch (iOperation)
	{
		case '=':
			break;
		case '+':
			break;
		case '*':
			break;
		case '&':
			break;
		case '|':
			break;
		case '^':
			break;
		case '<':
			break;
		case '>':
			break;
		case '/':
			break;
		case '%':
			break;
		default:
			return 0;
			break;
	}
}



LRESULT CALLBACK WndProc(HWND hwnd , UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static BOOL bNewNumber = TRUE;
	static int iOperation = '=';
	static UINT iNumber, iFirstNum;
	HWND hButton;


	switch (iMsg)
	{
	case WM_KEYDOWN:
		if (wParam != VK_LEFT)
		{
			break;
		}
		wParam = VK_BACK;
	case WM_CHAR:
		if ((wParam = (WPARAM)CharUpper((TCHAR*)wParam)) == VK_RETURN)
			wParam = '=';
		if (hButton = GetDlgItem(hwnd, wParam))
		{
			SendMessage(hButton, BM_SETSTATE, 1, 0);
			Sleep(100);
			SendMessage(hButton, BM_SETSTATE, 0, 0);
		}
		else
		{
			MessageBeep(0);
			break;
		}
	case WM_COMMAND:
		SetFocus(hwnd);

		if (LOWORD(wParam) == VK_BACK)			//backspace
		{
			ShowNumber(hwnd, iNumber /= 16);
		}
		else if(LOWORD(wParam) == VK_ESCAPE)	//escape
		{
			ShowNumber(hwnd, iNumber = 0);
		}
		else if (isxdigit(LOWORD(wParam)))		//hex Number
		{
			if (bNewNumber)
			{
				iFirstNum = iNumber;
				iNumber = 0;
			}
			bNewNumber = FALSE;

			if (iNumber <= MAXDWORD >> 4)
			{
				ShowNumber(hwnd, iNumber = 16 * iNumber + wParam - (isdigit(wParam) ? '0' : 'A' - 10));
			}
			else
			{
				MessageBeep(0);
			}
		}
		else
		{

		}if (!bNewNumber)
		{
			ShowNumber(hwnd, iNumber = calculator(iFirstNum, iOperation, iNumber));
			bNewNumber = TRUE;
			iOperation = LOWORD(wParam);
		}
		return 0;;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}





