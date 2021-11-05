#include <Windows.h>	
#include "registry.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>


//internal helper function prototypes
//Set the given key and its value

BOOL setKeyandValue(const char* pszPath, const char* szSubkey, const char* szValue);

//Convert a CLSID into a char string 

void CLSIDtochar(const CLSID& clsid, char * szCLSID, int length);

//Delete szKeyChild and all of its descendents

LONG recursiveDeleteKey(HKEY hKeyParentm, const char *szKeyChild);

//constants
//size of a CLSID as a string

const int CLSID_STRING_SIZE = 39;

//public function implementation
//Register the component in the registry 

HRESULT RegisterServer(HMODULE hModule, const CLSID& clsid, const char* szFriendlyName, const char*szVerIndProgID, const char*szProgID)
{
	//get server location
	char szModule[512];

	//This function returns fullpath and filename of the executable file containing the specific module
	DWORD dwResult = GetModuleFileNameA(hModule, szModule, (sizeof(szModule)/sizeof(char)));

	if (dwResult == 0)
	{
		std::cout << "RegisterServer:\t\t Failed to get module " << std::endl;
		return dwResult;
	}	

	//convert CLSID into char 
	char szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(clsid, szCLSID, sizeof(szCLSID));

	//Build the key CLSID\\{...}
	char szKey[64];
	strcpy(szKey, "CLSID\\");
	strcat(szKey, szCLSID);
	

	//Add the CLSID to the registry
	setKeyandValue(szKey, NULL, szFriendlyName);

	//Add server filename subkey under CLSID key
	setKeyandValue(szKey, "InprocServer32", szModule);

	//Add ProgID subkey under CLSID key 
	setKeyandValue(szKey, "ProgID", szProgID);

	//Add the version independent progID subkey 	under CLSID key
	setKeyandValue(szKey, "VersionIndependentProgID", szVerIndProgID);

	return S_OK;
}

//Remove the component from the registry

LONG UnregisterServer(const CLSID& clsid, const char *szVerIndProgID, const char* szProgID)
{
	
	//Convert the CLSID into char
	char szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(clsid, szCLSID, sizeof(szCLSID));

	//build the key CLSID\\{...}
	char szKey[64];
	strcpy(szKey, "CLSID\\");
	strcat(szKey, szCLSID);

	//Delete the CLSID key CLSID\{...}
	LONG lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szKey);
	if (lResult != ERROR_SUCCESS)
	{
		std::cout << "Unregister Server: failed to DeleteSZKey" << std::endl;
	}

	//Delete version independent prog key
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szVerIndProgID);
	if (lResult != ERROR_SUCCESS)
	{
		std::cout << "Unregister Server: failed to DeleteVERINDPROGKey" << std::endl;
	}
	
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szProgID);
	if (lResult != ERROR_SUCCESS)
	{
		std::cout << "Unregister Server: failed to DeletePROGKey" << std::endl;
	}

	return S_OK;
}



//convert CLSID to char string
void CLSIDtochar(const CLSID& clsid, char *szCLSID, int length)
{
	//Get CLSID
	LPOLESTR wszCLSID = NULL;
	HRESULT hr = StringFromCLSID(clsid, &wszCLSID);
	
	if (!SUCCEEDED(hr))
	{

		std::cout << "CLSIDtochar:\t\t failed to string clsid" << std::endl;
	}

	//convert wide char to non-wide char
	wcstombs(szCLSID, wszCLSID, length);

	//Freememory
	CoTaskMemFree(wszCLSID);

}


//Create a key and set its value

BOOL setKeyandValue(const char* szKey, const char*szSubKey, const char*szValue)
{
	void *pMsgBuf;

	HKEY hKey;
	char szKeyBuf[1024];
	//copy keyname into buffer
	strcpy(szKeyBuf, szKey);

	//Add subkey name to buffer
	if (szSubKey != NULL)
	{
		strcat(szKeyBuf, "\\");
		strcat(szKeyBuf, szSubKey);
	}

	//Create and open key and subkey

	HRESULT lResult = RegCreateKeyExA(HKEY_CLASSES_ROOT,szKeyBuf, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, lResult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&pMsgBuf, 0, NULL);
	std::cout <<"error: "<< (LPSTR)pMsgBuf << std::endl;
	LocalFree(pMsgBuf);

	//Set the value
	if (szValue != NULL)
	{
		RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE*)szValue, strlen(szValue) + 1);
	}
	RegCloseKey(hKey);

	return TRUE;
}



LONG recursiveDeleteKey(HKEY hKeyParent, const char *lpszKeyChild)
{	
	//open the child
	HKEY hKeyChild;
	LONG lRes = RegOpenKeyExA(hKeyParent, lpszKeyChild, 0, KEY_ALL_ACCESS, &hKeyChild);

	if (lRes != ERROR_SUCCESS)
	{
		return lRes;
	}

	//Enumerate all the decesdents of this child
	FILETIME time;
	char szBuffer[256];
	DWORD dwSize = 256;

	while (RegEnumKeyExA(hKeyChild, 0, szBuffer, &dwSize, NULL, NULL, NULL, &time) == S_OK)
	{
		//Delete the decendents	of this child
		//call with child as a new parent
		lRes = recursiveDeleteKey(hKeyChild, (char*)szBuffer);
		if (lRes != ERROR_SUCCESS)
		{
			//Cleanup before exitting
			RegCloseKey(hKeyChild);
			return(lRes);
		}
		dwSize = 256;
	}

	//Close the child
	RegCloseKey(hKeyChild);
	//Delete this child
	return RegDeleteKeyA(hKeyParent, lpszKeyChild);
}
