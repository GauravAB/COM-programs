#include <windows.h>
#include "resource.h"



BOOL CALLBACK dlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	void evaluate(unsigned int);

	HWND hOK;
	static unsigned int current_number = 0;
	switch (message)
	{
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hDlg, IDOK));
		break;
	case WM_COMMAND:
		switch (wParam)
		{
			case ID_0:
				current_number = 0;
				evaluate(current_number);
				break;
			case ID_1:
				current_number = 1;
				evaluate(current_number);
				break;
			case ID_2:
				current_number = 2;
				evaluate(current_number);
				break;
			case ID_3:
				current_number = 3;
				evaluate(current_number);
				break;
			case ID_4:
				current_number = 4;
				evaluate(current_number);
				break;
			case ID_5:
				current_number = 5;
				evaluate(current_number);
				break;
			case ID_6:
				current_number = 6;
				evaluate(current_number);
				break;
			case ID_7:
				current_number = 7;
				evaluate(current_number);
				break;
			case ID_8:
				current_number = 8;
				evaluate(current_number);
				break;
			case ID_9:
				current_number = 9;
				evaluate(current_number);
				break;
			case IDOK:
				EndDialog(hDlg, 0);
				break;
			case IDCANCEL:
				EndDialog(hDlg, 0);
				break;
		}
		break;
	default:
		break;
	}
	return FALSE;
}



void evaluate(unsigned int x)
{
	int base = 10;
	if ((x / 10) < 10)
	{
		base = 1;
	}
	else
	{
		base = 10;
	}

	x = x*base;
}



