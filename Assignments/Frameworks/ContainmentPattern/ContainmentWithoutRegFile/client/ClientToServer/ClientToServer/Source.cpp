#include <Windows.h>
#include <stdio.h>
#include "ContainmentOuterServer.h"
#include "Util.h"


void trace(const char * msg)
{
	Util::Trace("client", msg, S_OK);
}


void trace(const char*msg, HRESULT hr)
{
	Util::Trace("client", msg, hr);
}


int main(void)
{
	HRESULT hr;
	ISum* pISum = NULL;
	ISub* pISub = NULL;
	IDivision* pIDiv;
	IMultiplication* pIMul;
	int n1, n2, n3;

	TCHAR sysPath[MAX_PATH];
	TCHAR str[255];
	
	STARTUPINFO stInfo;
	PROCESS_INFORMATION pInfo;
	ZeroMemory(&stInfo, sizeof(STARTUPINFO));
	
	stInfo.cb = sizeof(STARTUPINFO);
	stInfo.dwFlags = STARTF_USESHOWWINDOW;
	stInfo.wShowWindow = SW_SHOWDEFAULT;

	trace("Initiating COM server");
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		trace("CoInitialize failed");
	}

	GetSystemDirectory(sysPath, MAX_PATH);
	wsprintf(str, L"%s\\regsvr32.exe ContainmentOuterServer.dll", sysPath);
	
	if (0 != (CreateProcess(NULL,str, NULL, NULL, FALSE, DETACHED_PROCESS | NORMAL_PRIORITY_CLASS, NULL, NULL,&stInfo,&pInfo)) )
	{
		GetSystemDirectory(sysPath, MAX_PATH);
		wsprintf(str, L"%s\\regsvr32.exe ContainmentInnerServer.dll", sysPath);
		if (0 != (CreateProcess(NULL, str, NULL, NULL, FALSE, DETACHED_PROCESS | NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo)))
		{

			hr = CoCreateInstance(CLSID_CSumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void**)&pISum);

			if (FAILED(hr))
			{
				trace("CoCreateInstance failed", hr);
				return 1;
			}
			else
			{
				n1 = 11;
				n2 = 12;
				pISum->additionOfTwoIntegers(n1, n2, &n3);

				cout << "addition of " << n1 << "and " << n2 << "is " << n3 << endl;

				hr = pISum->QueryInterface(IID_IDiv, (void**)&pIDiv);
				
				if (FAILED(hr))
				{
					trace("pIDiv failed");
					return 1;
				}

				n1 = 100;
				n2 = 2;
				if (pIDiv)
				{
					pIDiv->divisionOfTwoIntegers(n1, n2, &n3);
					cout << "Division of " << n1 << "and " << n2 << "is " << n3 << endl;

					hr = pIDiv->QueryInterface(IID_IMul, (void**)&pIMul);
					if (FAILED(hr))
					{
						trace("IMul get failed");
						return  1;
					}
					else
					{
						pIMul->multiplicationOfTwoIntegers(n1, n2, &n3);
						cout << "Multiplication of " << n1 << "and " << n2 << "is " << n3 << endl;
					}

					hr = pIMul->QueryInterface(IID_ISub, (void**)&pISub);
					if (FAILED(hr))
					{
						trace("ISub get failed");
						return  1;
					}
					else
					{
						pISub->subtractionOfTwoIntegers(n1, n2, &n3);
						cout << "subtraction of " << n1 << "and " << n2 << "is " << n3 << endl;
					}

				}
				else
				{
					trace("pIDiv failed");
				}


				pISub->Release();
				pIMul->Release();
				pIDiv->Release();
				pISum->Release();
			}
		}
	}
	else
	{
		trace("createprocess failed");
	}

	/*
	GetSystemDirectory(sysPath, MAX_PATH);
	wsprintf(str, L"%s\\regsvr32.exe -u ContainmentInnerServer.dll", sysPath);
	CreateProcess(NULL, str, NULL, NULL, FALSE, DETACHED_PROCESS | NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo);
	
	GetSystemDirectory(sysPath, MAX_PATH);
	wsprintf(str, L"%s\\regsvr32.exe -u ContainmentOuterServer.dll", sysPath);
	CreateProcess(NULL, str, NULL, NULL, FALSE, DETACHED_PROCESS | NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo);
	*/

	return 0;
}