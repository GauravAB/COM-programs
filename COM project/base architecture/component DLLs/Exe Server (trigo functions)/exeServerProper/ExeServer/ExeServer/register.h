#ifndef __REGISTER_H_
#define __REGISTER_H_

#include<Windows.h>
#include "Util.h"

const int CLSID_STRING_SIZE = sizeof(CLSID) / sizeof(TCHAR);

STDAPI RegisterServer(TCHAR *szClassDescription, HMODULE hModule, const CLSID& clsid);
STDAPI  UnregisterServer(const CLSID &clsid);

#endif