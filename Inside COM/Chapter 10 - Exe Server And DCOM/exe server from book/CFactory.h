#ifndef _CFACTORY_H__
#define _CFACTORY_H__

#include "CUnknown.h"

//Forward reference
class CFactoryData;

//Global data used by CFactory
extern CFactoryData g_FactoryDataArray[];
extern int g_cFactoryDataEntries;


//Component creation function
class CUnknown;

typedef HRESULT(*FPCREATEINSTANCE)(IUnknown*, CUnknown**);


//CF98/+98+ata
	//contains all information needed by cfactory to create component supported by DLL

class CFactoryData
{
public:
	//The class ID for the Component
	const CLSID* m_pCLSID;

	//Pointer to the function that creates it
	FPCREATEINSTANCE CreateInstance;


	//Name of the component to register in the registry
	const char* m_RegistryName;

	//ProgID
	const char* m_szProgID;

	//Version independent ProgID
	const char* m_szVerIndProgID;

	//Helper function to find class ID 
	BOOL IsClassID(const CLSID& clsid) const
	{
		return (*m_pCLSID == clsid);
	}


	//out of process server support

	//Pointer to running class factory for this component
	IClassFactory* m_pIClassFactory;

	//Magic cookie to identify running object
	DWORD m_dwRegister;
};


//Class Factory


class CFactory : public IClassFactory
{
public:
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);
	
	//IClassfactory
	virtual HRESULT __stdcall CreateInstance(IUnknown *pUnknownOuter, const IID &iid, void **ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);

	CFactory(const CFactoryData* pFactoryData);
	~CFactory(){}

	//DllGetclassobject Support
	static HRESULT GetClassObject(const CLSID &clsid ,const IID &iid, void **ppv);

	//helper function for DllCanUnload now
	static BOOL isLocked() { return (s_cServerLocks > 0); }

	//Functions to register and unregister all components
	static HRESULT RegisterAll();
	static HRESULT UnRegisterAll();

	//function to determine if the function can be unloaded
	static HRESULT CanUnloadNow();

#ifdef _OUTPROC_SERVER_
	//out of process server support

	static BOOL StartFactories(void);
	static void StopFactories(void);

	static DWORD s_dwThreadID;

	//shut down the application
	static void CloseExe()
	{
		if (CanUnloadNow() == S_OK)
		{
			::PostThreadMessage(s_dwThreadID, WM_QUIT, 0, 0);
		}
	}
#else
	//Close exe doesnt do anything if we are in process server
	static void CloseExe() {}
#endif

public:
	long m_cRef;

	//Pointer to information about class this factory creates
	const CFactoryData* m_pFactoryData;

	//count of locks
	static long s_cServerLocks;

	//module handle
	static HMODULE s_hModule;
};

#endif