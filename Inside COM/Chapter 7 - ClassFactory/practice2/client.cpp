#include<windows.h>
#include <iostream>
#include "interface.h"


void trace(const char * msg) {std::cout <<msg<<std::endl;}

int main(void)
{	
	CoInitialize(NULL);

	ISum *pISum = NULL;

	HRESULT hr = CoCreateInstance(CLSID_CMath,NULL,CLSCTX_INPROC_SERVER,IID_ISum,(void**)&pISum);

	if(SUCCEEDED(hr))
	{
		trace("success");
		trace("using ISum");
		int *res = (int*)malloc(sizeof(int));
		pISum->SumOfTwoIntegers(10,10,res);
		std::cout<<*res<<std::endl;
		
		trace("get ISubtract from ISum");
		ISubtract *pISub = NULL;

		hr = pISum->QueryInterface(IID_ISubtract,(void**)&pISub);
		if(SUCCEEDED(hr))
		{
			trace("ISubtract success");
			pISub->SubtractionOfTwoIntegers(20,10,res);
			std::cout<<*res<<std::endl;
		}
	}
	else
	{
		trace("failed to get the interface");
	}

	return 0;
} 