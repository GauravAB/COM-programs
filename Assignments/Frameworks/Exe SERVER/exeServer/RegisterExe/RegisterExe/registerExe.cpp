#include <Windows.h>
#include "register.h"



HMODULE ghModule;


BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}

	return TRUE;
}



STDAPI DllRegisterServer()
{
	return RegisterServer(L"ExeServerCom",)
}

STDAPI DllUnregisterServer()
{

}