//CLASSFACTORY CLASS DECLARATION

#ifndef __CFACTORY_H_
#define __CFACTORY_H_




class CFactory : public IClassFactory
{
public:
	//IUnknown Methods
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void** ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//IClassfactory interface methods

	virtual HRESULT __stdcall CreateInstance(IUnknown *pUnknownOuter, const IID &iid, void** ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);

	//CFactory methods
	CFactory(void);
	~CFactory(void);
private:
	long m_cRef;
};


#endif