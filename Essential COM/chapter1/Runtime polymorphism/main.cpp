
extern "C" IFastString* CallCreateFastString(const char *psz , bool bLeftToRight  = true)
{
	//polymorphic funtionn pointers
	static IFastString*(*pfnlr)(const char*) =0;
	static IFastString*(*pfnrl)(const char*) =0;

	//pointer to funtion pointer
	//keep default pointer to lr
	IFastString*(**ppfn)(const char*) = &pfnlr
	//load dll  lr string read setup
	const TCHAR *pszDll  = __TEXT("FastString.DLL");

	//but if not 
	if(!bLeftToRight)
	{
		//load the other dll
		pszDll = __TEXT("FastStringRL.DLL");
		
		//change the pointer to rl funtion pointer
		ppfn = &pfnrl;
	}

	//if pointer to funtion is empty i.e first use
	if(!(*ppfn))
	{
		const char szFn[]  = "CreateFastString";
		//load the required library
		HINSTANCE h = LoadLibrary(pszDll);

		//Extract the funtion required
		if(h)
		{
			*(FARPROC*)ppfn = GetProcAddress(h,szFn);
		}
	}
	
	//return by calling createinstance with required parameter
	return (*ppfn) ? (*ppfn)(psz):0;
}
