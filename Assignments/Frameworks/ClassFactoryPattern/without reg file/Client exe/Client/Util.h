#pragma once
#include <iostream>

using std::cout;
using std::endl;

namespace Util
{
	char *szBuffer = NULL;
	char str[255];

	void Handle(const char *, const char*, HRESULT);

	void Trace(const char*szLabel, const char* msg, HRESULT hr)
	{
		if (hr == S_OK)
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
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPSTR)&szBuffer, 0, NULL);

		wsprintfA(str, "%#x %s", hr, szBuffer);
		cout << szLabel<<"\t"<<msg<<"\t"<< szBuffer << endl;
		LocalFree(szBuffer);
	}

};