#include <Windows.h>
#include <stdio.h>
#include "AggregationOuterServer.h"
#include "Util.h"

int myCreateProcess(TCHAR*szPath);

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
	ISquare* pISquare = NULL;
	ISquareroot* pISquareroot = NULL;
	
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
	wsprintf(str, L"%s\\regsvr32.exe AggregationOuterServer.dll", sysPath);

	if (0 != (CreateProcess(NULL,str, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL,&stInfo,&pInfo)) )
	{
			GetSystemDirectory(sysPath, MAX_PATH);
			wsprintf(str, L"%s\\regsvr32.exe AggregationInnerServer.dll", sysPath);
			CreateProcess(NULL, str, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo);

			hr = CoCreateInstance(CLISD_CSquare, NULL, CLSCTX_INPROC_SERVER, IID_ISquare, (void**)&pISquare);

			
			if (FAILED(hr))
			{
				trace("CoCreateInstance failed", hr);
				return 1;
			}
			else
			{
				n1 = 2;
				//n3 = 2;
				pISquare->SquareOfAnInteger(n1, &n3);
				cout << "square of " << n1 << " is " << n3 << endl;
					
				hr = pISquare->QueryInterface(IID_ISquareroot, (void**)&pISquareroot);
					if (FAILED(hr))
					{
						trace("ISquareroot failed to get");
					}
					else
					{
						int res;
						pISquareroot->SquarerootOfAnInteger(144,&res);
						cout << "squareroot of 144" << " is " << res << endl;	
						pISquareroot->Release();
					}	
				}
				
				pISquare->Release();
	}
	else
	{
		trace("createprocess failed");
	}

	GetSystemDirectory(sysPath, MAX_PATH);
	wsprintf(str, L"%s\\regsvr32.exe -u AggregationInnerServer.dll", sysPath);
	CreateProcess(NULL, str, NULL, NULL, FALSE,NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo);
	GetSystemDirectory(sysPath, MAX_PATH);
	wsprintf(str, L"%s\\regsvr32.exe -u AggregationOuterServer.dll", sysPath);
	CreateProcess(NULL, str, NULL, NULL, FALSE,NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo, &pInfo);
	

	return 0;
}