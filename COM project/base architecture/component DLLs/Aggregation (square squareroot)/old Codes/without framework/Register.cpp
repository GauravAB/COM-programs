#include <Windows.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include "RegisterAPI.h"
#include "Util.h"

static inline void trace(const char* msg)
{
	Util::Trace("RegisterAPI", msg, S_OK);
}
static inline void trace(const char* msg, HRESULT hr)
{
	Util::Trace("RegisterAPI", msg, hr);
}


//set the given key and value
BOOL setKeyandValue(const char* pszPath, const char* szSubKey, const char*szValue);

//CLSIDtochar
void CLSIDtochar(const CLSID&clsid, char* szCLSID, int length);

//Delete Key and all of its subkeys
LONG recursiveDeleteKey(HKEY hKeyParentm, const char *szKeyChild);

//size of CLSID as a string
const int CLSID_STRING_SIZE = 39;

//Register Component in the registry

HRESULT RegisterServer(HMODULE hModule, const CLSID &clsid, const char*szFriendlyName, const char*szVerIndProgID, const char*szProgID)
{
	char szModule[512];

	DWORD dwResult = GetModuleFileName(hModule, szModule, sizeof(szModule)/sizeof(char) );

	if (dwResult == 0)
	{
		trace("GetModuleFileName failed");
		return dwResult;
	}

	//convert CLSID to char
	char szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(clsid, szCLSID, sizeof(szCLSID));

	//build the key CLSID
	char szKey[64];
	strcpy(szKey, "CLSID\\");
	strcat(szKey, szCLSID);

	//Add the CLSID to the registry
	setKeyandValue(szKey, NULL, szFriendlyName);

	//Add server filename subkey under CLSID
	setKeyandValue(szKey, "InprocServer32", szModule);
	
	//Add progID subkey under CLSID key
	setKeyandValue(szKey, "ProgID", szProgID);
	
	//Add the version independent progID subkey under CLSID key
	setKeyandValue(szKey, "VerIndProgID", szVerIndProgID);

	return S_OK;
}


void CLSIDtochar(const CLSID &clsid, char *szCLSID, int length)
{

	//wchar_t string pointer
	LPOLESTR wszCLSID = NULL;
	HRESULT hr = StringFromCLSID(clsid, &wszCLSID);
	

	if (FAILED(hr))
	{
		trace("CLSIDtochar failed to string clsid");
	}
	
	//convert wide char string to char string
	wcstombs(szCLSID, wszCLSID, length);

	CoTaskMemFree(wszCLSID);
}


LONG UnregisterServer(const CLSID &clsid, const char* szVerIndProgID, const char* szProgID)
{
	char szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(clsid, szCLSID, sizeof(szCLSID));

	//build the key CLSID
	char szKey[64];
	strcpy(szKey, "CLSID\\");
	strcat(szKey, szCLSID);

	//Delete the CLSID key CLSID\{..} 
	LONG lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szKey);
	

	if (lResult != ERROR_SUCCESS)
	{
		trace("UnRegister server", lResult);
	}
	//Delete Version independent prog key

	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szVerIndProgID);

	if (lResult != ERROR_SUCCESS)
	{
		trace("UnRegister server VerIndProgID", lResult);
	}

	//Delete Version progID
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szProgID);


	if (lResult != ERROR_SUCCESS)
	{
		trace("UnRegister server ProgID", lResult);
	}

	return S_OK;


}







BOOL setKeyandValue(const char* szKey, const char*szSubKey, const char*szValue)
{
	void *pMsgBuf;
	HKEY hKey;
	char szKeyBuf[1024];
	
	//copy keyname into buffer
	strcpy(szKeyBuf, szKey);

	//add subkey name to buffer
	if (szSubKey != NULL)
	{
		strcat(szKeyBuf, "\\");
		strcat(szKeyBuf, szSubKey);
	}

	//Create/open Registered Key

	HRESULT lResult = RegCreateKeyEx(HKEY_CLASSES_ROOT, szKeyBuf, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if (FAILED(lResult))
	{
		trace("setKeyAndValue", lResult);
	}

	if (szValue != NULL)
	{
		RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE*)szValue, strlen(szValue) + 1);
	}

	RegCloseKey(hKey);

	return TRUE;
}




LONG recursiveDeleteKey(HKEY hKeyParent, const char *lpszKeyChild)
{
	HKEY hKeyChild;
	LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyChild, 0, KEY_ALL_ACCESS, &hKeyChild);

	if (lRes != ERROR_SUCCESS)
	{
		return lRes;
	}

	//Enumerate all the decesdents of the child 
	
	FILETIME time;
	char szBuffer[256];
	DWORD dwSize = 256;

	while (RegEnumKeyEx(hKeyChild,0,szBuffer,&dwSize,NULL,NULL,NULL,&time) == S_OK)
	{
		 //Delete the descendents of the child
		//call with child as new parent

		lRes = recursiveDeleteKey(hKeyChild, szBuffer);
		if (lRes != ERROR_SUCCESS)
		{
			RegCloseKey(hKeyChild);
			return(lRes);
		}
		dwSize = 256;
	}
	RegCloseKey(hKeyChild);
	return RegDeleteKey(hKeyParent, lpszKeyChild);
}






