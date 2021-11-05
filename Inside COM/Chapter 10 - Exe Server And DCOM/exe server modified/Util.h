#ifndef _Util_h__
#define _Util_h__
#include <Windows.h>
namespace Util
{
	void Trace(const char* szLabel, const char* szText, HRESULT hr);
	void ErrorMessage(HRESULT hr);
};

#endif	//_Util_h__




