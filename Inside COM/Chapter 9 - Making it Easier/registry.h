#ifndef __Registry_H__
#define __Registry_H__

//function which registers the component into the registry
HRESULT RegisterServer(HMODULE hModule, const CLSID& clsid, const char *szFriendlyName, const char *szVerIndProgID, const char *szProgID);

//this function unregisters the component
HRESULT UnregisterServer(const CLSID& clsid, const char *szVerIndProgID, const char *szProgID);

#endif