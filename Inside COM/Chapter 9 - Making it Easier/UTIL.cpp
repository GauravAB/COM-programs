#include <stdlib.h>
#include <iostream>
#include <Windows.h>
using namespace std;	

namespace Util
{

	void ErrorMessage(HRESULT hr)
	{
		void *pMsgBuf;

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pMsgBuf, 0, NULL);
		cout << "Error (" << hex << hr << "):" << (LPTSTR)pMsgBuf << endl;

		LocalFree(pMsgBuf);
	}

	void Trace(char *szLabel, const char *szText, HRESULT hr)
	{
		cout << szLabel << ": \t" << szText << endl;

		if (FAILED(hr))
		{
			ErrorMessage(hr);
		}
	}


};