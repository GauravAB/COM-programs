
extern "C" IFastString* CallCreateFastString(const char *psz , bool bLeftToRight  = true)
{
	//polymorphic funtionn pointers
	static IFastString*(*pfnlr)(const char*) =0;
	static IFastString*(*pfnrl)(const char*) =0;

	//pointer to funtion pointer
	//keep default pointer to lr
	IFastString*(**ppfn)(const char*) = &pfnlr
	//load dll with lr string read setup
	const TCHAR *pszDll  = __TEXT("FastString.DLL");

	if(!bLeftToRight)
	{
		pszDll = __TEXT("FastStringRL.DLL");
		
		ppfn = &pfnrl;
	}

	if(!(*ppfn))
	{
		const char szFn[]  = "CreateFastString";
		HINSTANCE h = LoadLibrary(pszDll);
		if(h)
		{
			*(FARPROC*)ppfn = GetProcAddress(h,szFn);
		}
	}
	
	return (*ppfn) ? (*ppfn)(psz):0;
}
