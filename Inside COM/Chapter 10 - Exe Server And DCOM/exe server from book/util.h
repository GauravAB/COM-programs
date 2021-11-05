#ifndef _UTIL_H__
#define _UTIL_H__

namespace Util
{
	void Trace(char* szLabel, char *szText, HRESULT hr);
	void ErrorMessage(HRESULT hr);

};

//ostream& operator << (ostream& os, const wchar_t* wsz);

#endif _UTIL_H__