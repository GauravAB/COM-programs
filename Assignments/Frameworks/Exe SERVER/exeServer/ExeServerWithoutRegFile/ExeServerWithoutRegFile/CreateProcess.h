#ifndef __CREATE_PROCESS_H
#define __CREATE_PROCESS_H
#include <windows.h>

extern void trace(const char*msg);


int myCreateProcess(TCHAR*szPath)
{

	STARTUPINFO stInfo;
	PROCESS_INFORMATION pInfo;

	ZeroMemory(&stInfo, sizeof(stInfo));
	ZeroMemory(&pInfo, sizeof(pInfo));

	stInfo.cb = sizeof(STARTUPINFO);
	stInfo.dwFlags = STARTF_USESHOWWINDOW;
	stInfo.wShowWindow = SW_SHOWDEFAULT;


	if (0 == CreateProcess(NULL, szPath, NULL, NULL, NULL, HIGH_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo))
	{
		trace("CreateProcess failed");
		return 0;
	}
	else
	{
		WaitForInputIdle(pInfo.hProcess, INFINITE);
	}

	return 1;

}




#endif