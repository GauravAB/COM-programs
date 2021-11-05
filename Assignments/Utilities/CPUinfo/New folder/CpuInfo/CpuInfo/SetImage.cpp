#pragma once
#include <Windows.h>

HBITMAP SetupBackGroundImage(HWND hwnd, DWORD ID)
{
	HBITMAP hBitmap;
	HBITMAP hBitmapRec;
	//Load Image
	//open the clipboard corresponding to the main process
	OpenClipboard(hwnd);
	//empty the clipboard 
	EmptyClipboard();
	//Load the bitmap
	hBitmap = LoadBitmap((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(ID));

	//placing data on the clipboard to specified clipboard format i.e in this case to BitMap
	if (SetClipboardData(CF_BITMAP, hBitmap) == NULL)
	{
		MessageBox(NULL, TEXT("Could not set the desired data\n"), NULL, MB_ICONERROR | MB_OK);
		CloseClipboard();
	}

	CloseClipboard();

	OpenClipboard(hwnd);

	//get the bitmap from the clipboard
	if ((hBitmapRec = (HBITMAP)GetClipboardData(CF_BITMAP)) == NULL)
	{
		MessageBox(hwnd, TEXT("Can not access desired data."), NULL, MB_ICONERROR | MB_OK);
		CloseClipboard();
	}
	CloseClipboard();

	//invalidate client region for paint
	InvalidateRect(hwnd, NULL, TRUE);

	return (hBitmapRec);
}



