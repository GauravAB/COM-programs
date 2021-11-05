#ifndef __CFactory_h__
#define __CFactory_h__
#include "CUnknown.h"


class CFactory : public IClassFactory
{
public:
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//IClassFactory
	virtual HRESULT __stdcall CreateInstance(IUnknown* pUnknownOuter, const IID &iid, void**ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);

	CFactory(void);
	~CFactory(void);
	
	static HRESULT StartFactories(void);
	static void StopFactories(void);
    static DWORD s_dwThreadID;
	static long s_cServerLocks;

private:
	long m_cRef;

};


#endif
