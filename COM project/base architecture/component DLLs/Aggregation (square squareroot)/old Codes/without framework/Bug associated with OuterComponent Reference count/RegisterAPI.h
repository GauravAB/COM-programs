#ifndef __RegServer_h
#define __RegServer_h

HRESULT RegisterServer(HMODULE hModule,const CLSID& clsid ,const char*friendlyname,const char*versionIndProgID,const char*progID);
HRESULT UnregisterServer(const CLSID& clsid,const char*versionIndProgID,const char*progID);



#endif