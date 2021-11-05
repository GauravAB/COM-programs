#ifndef _REGISTRY_H
#define _REGISTRY_H
#include <windows.h>


HRESULT RegisterServer(HMODULE hModule ,const CLSID &clsid , const char* szFriendlyName , const char* szVerIndProgID , const char* szProgID);
HRESULT UnregisterServer(const CLSID &clsid ,const 	char* szVerIndProgID ,const char* szProgID);

#endif







