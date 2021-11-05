#ifndef _Registry_h
#define _Registry_h



HRESULT RegisterServer(HMODULE hModule, const CLSID &clsid, const char*szFriendlyName, const char*szVerIndProgID, const char*szProgID);
HRESULT UnregisterServer(const CLSID &clsid, const char*szVerIndProgID, const char*szProgID);


#endif

















