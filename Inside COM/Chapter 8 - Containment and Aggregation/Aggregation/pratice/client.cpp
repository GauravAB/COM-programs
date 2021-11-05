#include <windows.h>
#include <iostream>
#include "interface.h"


void trace(const char *msg) {std::cout << msg << std::endl;}

int main(void)
{

	void *pMsgBuf;


	CoInitialize(NULL);

	IX* pIX = NULL;

	HRESULT hr = CoCreateInstance(CLSID_CA,NULL,CLSCTX_INPROC_SERVER,IID_IX,(void**)pIX);

	if(FAILED(hr))
	{
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER| FORMAT_MESSAGE_FROM_SYSTEM,NULL,hr,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR)&pMsgBuf,0,NULL);
		
		trace("failed to create instance");
		std::cout<<"error:"<<" "<<(LPSTR)pMsgBuf<<std::endl;
		LocalFree(pMsgBuf);
	}
	else
	{

	}

	return 0;
}






