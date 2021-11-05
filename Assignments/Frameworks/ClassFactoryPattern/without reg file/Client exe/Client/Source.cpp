#include <Windows.h>
#include "ClassFactoryServerWithoutRegFile.h"
#include "Util.h"
#include <iostream>

using std::cout;
using std::endl;

void trace(const char *msg, HRESULT hr)
{
	Util::Trace("Client", msg, hr);
}
void trace(const char* msg)
{
	Util::Trace("Client", msg, S_OK);
}


int main(void)
{
	HRESULT hr;
	ISum *pISum = NULL;
	TCHAR szPath[MAX_PATH];
	TCHAR str[255];
	STARTUPINFO stInfo;
	PROCESS_INFORMATION pInfo;
	int num1, num2, num3;

	ZeroMemory(&stInfo, sizeof(stInfo));
	ZeroMemory(&pInfo, sizeof(pInfo));

	stInfo.cb = sizeof(STARTUPINFO);
	stInfo.dwFlags = STARTF_USESHOWWINDOW;
	stInfo.wShowWindow = SW_SHOWDEFAULT;




	CoInitialize(NULL);

	GetSystemDirectory(szPath,sizeof(szPath));
	wsprintf(str, L"%s\\regsvr32.exe ClassFactoryWithoutRegFile.dll", szPath);
	CreateProcess(NULL,str, NULL, NULL, NULL, DETACHED_PROCESS | IDLE_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo);
		
	hr = CoCreateInstance(CLSID_CSumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISum,(void**) &pISum);
	
	if (FAILED(hr))
	{
		trace("CoCreateInstance failed", hr);
	}
	else
	{
		num1 = 25;
		num2 = 25;

		trace("Using pISum");
		pISum->SumOfTwoIntegers(num1,num2,&num3);
		cout << "sum of " << num1 << "and " << num2 << "is " << num3<<endl;
		pISum->Release();
	}

	//wsprintf(str, L"%s\\regsvr32.exe -u ClassFactoryWithoutRegFile.dll", szPath);
	//CreateProcess(NULL, str, NULL, NULL, NULL, DETACHED_PROCESS | IDLE_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo);

	return 0;
}

