#include "register.h"
#include "Util.h"
static void trace(const char* msg)
{
	Util::Trace("Registry: ", msg, S_OK);
}

static void trace(const char* msg, HRESULT hr)
{
	Util::Trace("Registry: ", msg, hr);
}


STDAPI RegisterServer(TCHAR *szClassDescription,HMODULE hModule ,const CLSID& clsid)
{

	HKEY hCLSIDKey = NULL;
	HKEY hInProcServerKey = NULL;
	LONG lRet;
	TCHAR szModulePath[MAX_PATH];
	LPOLESTR stringCLSID;
	TCHAR str[] = L"Wow6432Node\\CLSID\\";
	TCHAR szKey[MAX_PATH];
	TCHAR szThreadingModel[] = L"Apartment";

	
	StringFromCLSID(clsid,&stringCLSID);
	wcscpy(szKey, str);
	wcscat(szKey,stringCLSID);

	__try
	{
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT, szKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, NULL, &hCLSIDKey, NULL);

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}

		lRet = RegSetValueEx(hCLSIDKey, NULL, 0, REG_SZ, (const BYTE*)szClassDescription, sizeof(szClassDescription));

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}

		//create the inprocserver32 key , which holds info about our coclass

		lRet = RegCreateKeyEx(hCLSIDKey, TEXT("LocalServer32"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hInProcServerKey, NULL);

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}

		//The default value of the inprocServer32 holds full dll path

		GetModuleFileName(hModule, szModulePath, MAX_PATH);

		lRet = RegSetValueEx(hInProcServerKey, NULL, 0, REG_SZ, (const BYTE*)szModulePath, sizeof(TCHAR)*(lstrlen(szModulePath) + 1));

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}
		//the threading model tells COM how to handle threads
		lRet = RegSetValueEx(hInProcServerKey, TEXT("ThreadingModel"), 0, REG_SZ, (const BYTE*)szThreadingModel, sizeof(szThreadingModel));

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}
	}

	__finally
	{
		if (NULL != hCLSIDKey)
		{
			RegCloseKey(hCLSIDKey);
		}

		if (NULL != hInProcServerKey)
		{

			RegCloseKey(hInProcServerKey);
		}
		if (stringCLSID)
		{
			CoTaskMemFree(stringCLSID);
		}
	}
	return S_OK;
}

STDAPI UnregisterServer(const CLSID &clsid)
{
	HRESULT hr;
	TCHAR postfix[] = L"\\LocalServer32";
	TCHAR szKey[MAX_PATH];
	TCHAR szSubKey[MAX_PATH];
	LPOLESTR stringCLSID;
	TCHAR str[] = L"Wow6432Node\\CLSID\\";

	StringFromCLSID(clsid, &stringCLSID);
	wcscpy(szKey, str);
	wcscat(szKey, stringCLSID);

	wcscpy(szSubKey, szKey);
	wcscat(szSubKey, postfix);

	hr = RegDeleteKey(HKEY_CLASSES_ROOT, szSubKey);
	
	if(hr != ERROR_SUCCESS)
	{
		trace("RegDeleteKey:szSubKey",hr);
	}

	hr = RegDeleteKey(HKEY_CLASSES_ROOT, szKey);

	if (hr != ERROR_SUCCESS)
	{
		trace("RegDeleteKey:szKey",hr);
	}

	if (stringCLSID)
	{
		CoTaskMemFree(stringCLSID);
	}
	
	return S_OK;
}
