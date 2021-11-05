#pragma once
#include <iostream>

using std::cout;
using std::endl;

namespace Util
{
	TCHAR *szBuffer = NULL;
	TCHAR str[255];

	void Handle(const char *, const char*, HRESULT);

	void Trace(const char*szLabel, const char* msg, HRESULT hr)
	{
		if (!FAILED(hr))
		{
			cout << szLabel << "\t" << msg << endl;
		}
		else
		{
			Handle(szLabel, msg, hr);
		}
	}

	void Handle(const char * szLabel, const char *msg, HRESULT hr)
	{
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, 0, NULL);
		wsprintf(str, L"%#x %s", hr, szBuffer);
		cout << szLabel << "\t" << msg << "\t" << str << endl;
	}

};