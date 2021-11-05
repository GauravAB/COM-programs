#ifndef __UTIL__H_
#define __UTIL__H_

namespace Util
{
	extern void Handle(const char *, const char*, HRESULT hr );
	extern void Trace(const char*szLabel, const char* msg, HRESULT hr);
};
	
#endif