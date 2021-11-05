#include<Windows.h>
#include <iostream>
#include "Util.h"

using std::cout;
using std::endl;

namespace Util
{
	TCHAR label[255];
	TCHAR buff[255];

	void ErrorMessage(HRESULT hr)
	{
		void *Tbuffer;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&Tbuffer, 0, NULL);
		wsprintf(buff, TEXT("Error:%s"), (LPSTR)Tbuffer);
		MessageBox(NULL, buff, TEXT("HRESULT FORMAT STRING"), MB_OK);
		//cout << "Error HRno:" << hr << (LPSTR)Tbuffer << endl;
		LocalFree(Tbuffer);
	}

	void Trace(const char*szLabel, const char*message, HRESULT hr)
	{
		
		if (FAILED(hr))
		{
			ErrorMessage(hr);
		}
		else
		{
			//cout << szLabel << ":\t" << message << endl;
			wsprintf(buff, TEXT("%s"),message);
			wsprintf(label, TEXT("%s"),szLabel);

			MessageBox(NULL, buff,label, MB_OK);

		}
	}
};

