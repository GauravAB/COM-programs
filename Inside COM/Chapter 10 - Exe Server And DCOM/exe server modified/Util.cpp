#include "Util.h"
#include <iostream>
#include <stdlib.h>

using std::cout;
using std::endl;

namespace Util
{

	void Util::Trace(const char* szLabel, const char* szText, HRESULT hr)
	{
		cout << szLabel << "\t" << szText << endl;
		if (FAILED(hr))
		{
			Util::ErrorMessage(hr);
		}
	}


	void Util::ErrorMessage(HRESULT hr)
	{
		void *pMsgBuf;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&pMsgBuf,
			0,
			NULL);

		cout << "Error (" << hr << "): " << (LPTSTR)pMsgBuf << endl;
		//Free the Buffer
		LocalFree(pMsgBuf);
	}

};

