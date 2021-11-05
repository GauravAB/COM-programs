#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "util.h"

#ifdef _OUTPROC_SERVER_
//if building local or remote server
	
	//listbox window handle
	extern HWND g_hWndListBox;

	static inline void output(const char* sz)
	{
		::SendMessage(g_hWndListBox, LB_ADDSTRING, 0, (LPARAM)sz);
	}
#else

//we are building an inProc server
#include <iostream>
	static inline void output(const char *sz)
	{
		std::cout << sz << std::endl;
	}
#endif

//utils

namespace Util
{
	//print out a message with label
	void Trace(char *szLabel, char* szText, HRESULT hr)
	{
		char buf[256];

		sprintf(buf, "%s: \t%s", szLabel, szText);
		output(buf);

		if (FAILED(hr))
		{
			ErrorMessage(hr);
		}

		//LocalFree(pMsgBuf);
	}

	void ErrorMessage(HRESULT hr)
	{
		void *pMsgBuf;

		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&pMsgBuf,
			0,
			NULL);

		char buf[256];

		sprintf(buf, "Error(%x: %s", hr, (char*)pMsgBuf);
		output(buf);

		//free the local buffer
		LocalFree(pMsgBuf);
	}
};
