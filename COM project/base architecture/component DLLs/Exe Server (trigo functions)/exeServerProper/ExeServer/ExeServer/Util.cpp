#include <Windows.h>
#include <iostream>
#include "Util.h"

using std::cout;
using std::endl;

namespace Util
{
	CHAR *szBuffer = NULL;
	CHAR str[255];

void Trace(const char*szLabel, const char* msg, HRESULT hr)
{
	if (!FAILED(hr))
	{
		wsprintfA(str, "%s %s ", szLabel, msg);
		MessageBoxA(NULL, str, "Util:Trace", MB_OK);
	}
	else
	{
		Handle(szLabel, msg, hr);
	}
}

void Handle(const char * szLabel, const char *msg, HRESULT hr)
{
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&szBuffer, 0, NULL);
	wsprintfA(str, "%s %#x %s", msg, hr, szBuffer);
	MessageBoxA(NULL, str, szLabel, MB_OK);
}

};
