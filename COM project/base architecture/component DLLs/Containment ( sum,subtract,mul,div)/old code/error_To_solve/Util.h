#ifndef __Util_H
#define __Util_H

namespace Util
{

	void Trace(const char *szLabel , const char* message , HRESULT hr);
	void ErrorMessage(HRESULT hr);
};


#endif


