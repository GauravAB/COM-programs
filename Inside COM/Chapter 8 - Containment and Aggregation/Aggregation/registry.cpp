#include <Windows.h>
#include "registry.h"
#include <stdlib.h>	
#include <string.h>
#include <iostream>

	
BOOL setKeyAndValue(const char *szPath, const char *szSubKey, const char * szValue);

void CLSIDtochar(const CLSID &clsid, char *szCLSID, int length);

LONG recursiveDeleteKey(HKEY hKeyParentm, const char*szKeyChild);

const int CLSID_STRING_SIZE = 39;


HRESULT RegisterServer(HMODULE hModule, const CLSID &clsid, const char *szFriendlyName, const char* szVerIndProgID, const char * szProgId)
{
	//Server location storage buffer
	char szModule[512];

	//This function finds the Dlls path along with its name
	DWORD dwResult = GetModuleFileName(hModule, szModule, (sizeof(szModule) / sizeof(char)));

	if (dwResult == 0)
	{
		std::cout << "RegisterServer : failed to get module path and name" << std::endl;
		return dwResult;
	}

	//convert CLSID into char
	char szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(clsid, szCLSID, sizeof(szCLSID));

	//Build the key
	char szKey[64];
	strcpy(szKey, "CLSID\\");
	strcat(szKey, szCLSID);

	//Add the CLSID to the registry
	setKeyAndValue(szKey, NULL, szFriendlyName);
	setKeyAndValue(szKey, "InProcServer32", szModule);
	setKeyAndValue(szKey, "ProgID", szProgId);
	setKeyAndValue(szKey, "VersionIndependentProgID", szVerIndProgID);

	return S_OK;

}

BOOL setKeyAndValue(const char *szKey, const char *szSubKey, const char *szValue)
{
	void *pMsgBuf;

	HKEY hKey;
	char szKeyBuf[1024];

	//copy keyname to buffer

	strcpy(szKeyBuf, szKey);	

	//Add subkey name to buffer
	if (szSubKey != NULL)
	{
		strcat(szKeyBuf, "\\");
		strcat(szKeyBuf, szSubKey);
	}
	
	//Create or open key or subkey
	
	HRESULT lResult = RegCreateKeyEx(HKEY_CLASSES_ROOT, szKeyBuf, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);


	if (lResult != ERROR_SUCCESS)
	{

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, lResult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pMsgBuf, 0, NULL);
		std::cout << "Error:" << (LPSTR)pMsgBuf << std::endl;
		LocalFree(pMsgBuf);
	}

	//Set The Value

	if (szValue != NULL)
	{
		RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE*)szValue, strlen(szValue) + 1);
	}

	RegCloseKey(hKey);

	return(TRUE);

}


LONG UnregisterServer(const CLSID &clsid, const char *szVerIndProgID, const char *szProgID)
{
	//Convert CLSID to Char
	char szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(clsid, szCLSID, sizeof(szCLSID));

	//build the key
	char szKey[64];
	strcpy(szKey, "CLSID\\");
	strcat(szKey, szCLSID);

	LONG lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szKey);

	if (lResult != ERROR_SUCCESS)
	{
		std::cout << "Unregister Server: failed to Delete VerIndProgkEy" << std::endl;
	}

	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szVerIndProgID);
	if (lResult != ERROR_SUCCESS)
	{
		std::cout << "Unregister Server: failed to Delete VerIndProgkEy" << std::endl;
	}

	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szProgID);
	
	if (lResult != ERROR_SUCCESS)
	{
		std::cout << "Unregister Server: failed to Delete VerIndProgkEy" << std::endl;
	}

	
	return(S_OK);
}


LONG recursiveDeleteKey(HKEY hKeyParent, const char *lpszKeyChild)
{
	HKEY hKeyChild;

	LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyChild, 0, KEY_ALL_ACCESS, &hKeyChild);

	if (lRes != ERROR_SUCCESS)
	{
		return lRes;
	}

	//Enumerate all the descendents of this child 

	FILETIME time;
	char szBuffer[256];
	DWORD dwSize = 256;

	while (RegEnumKeyEx(hKeyChild, 0, szBuffer, &dwSize, 0, 0, 0, &time) == S_OK)
	{
		lRes = recursiveDeleteKey(hKeyChild, szBuffer);

		if (lRes != ERROR_SUCCESS)
		{
			RegCloseKey(hKeyChild);
			return(lRes);
		}
		dwSize = 256;
	}

	//close the child 
	RegCloseKey(hKeyChild);

	return(RegDeleteKey(hKeyParent, lpszKeyChild));

}





void CLSIDtochar(const CLSID& clsid, char *szCLSID, int length)
{
	LPOLESTR wszCLSID = NULL;
	HRESULT hr = StringFromCLSID(clsid, &wszCLSID);

	if (hr == NULL)
	{
		std::cout << "CLSIDtoChar:\t\t failed to string clsid" << std::endl;
	}

	//convert wide char to non wide char

	wcstombs(szCLSID, wszCLSID, length);

	CoTaskMemFree(wszCLSID);
}










