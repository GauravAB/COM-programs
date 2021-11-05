#include<windows.h>
#include <iostream>
#include "interfaces.h"


void trace(const char * msg) {std::cout <<msg<<std::endl;}

//typedef STDAPI fp*(const CLSID &clsid, const IID &iid, void **ppv)

int main(void)
{	
	
	CoInitialize(NULL);

	ISum *pISum = NULL;
	ISubtract *pISub = NULL;
	void * pMsgBuf;

	HRESULT hr = CoCreateInstance(CLSID_CMath,NULL,CLSCTX_INPROC_SERVER,IID_ISum,(void**)&pISum);

	if(SUCCEEDED(hr))
	{
		trace("success");
		trace("using ISum");
		int res = 0;
		res = pISum->SumOfTwoIntegers(20,10);
		std::cout<<res<<std::endl;
		pISum->Release();
		CoUninitialize();
		// trace("asking for ISubtract");
		// hr = pISum->QueryInterface(IID_ISubtract,(void**)&pISub);
		// if(!FAILED(hr))
		// {
		// 	trace("ISubtract interface available now...");
		// 	res = pISub->SubtractionOfTwoIntegers(40,20);
		// 	std::cout<<res<<std::endl;

		// 	ISum *pSumfromSub = NULL;
		// 	trace("asking the aggregated component for ISum");
		// 	hr  = pISub->QueryInterface(IID_ISum,(void**)&pSumfromSub);
		// 	if(!FAILED(hr))
		// 	{
		// 		trace("aggregation complete!!");
		// 	}

		// }
	}
	else
	{
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER| FORMAT_MESSAGE_FROM_SYSTEM,NULL,hr,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR)&pMsgBuf,0,NULL);
		
		std::cout<<"error:"<<" "<<(LPSTR)pMsgBuf<<std::endl;
		LocalFree(pMsgBuf);

		trace("failed to get the interface");
	}

	return 0;
} 

