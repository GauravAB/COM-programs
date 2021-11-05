#ifndef __Util_h__
#define __Util_h__

namespace Util
{
	void Trace(char *szLabel, const char* szText, HRESULT hr);

	void ErrorMessage(HRESULT hr);
};

#endif