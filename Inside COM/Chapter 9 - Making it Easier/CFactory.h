#ifndef __CFactory_h
#define __CFactory_h

#include "CUnknown.h"


//Forward Reference
class CFactoryData;

//Global data used by CFactory
extern CFactoryData g_FactoryDataArray[];
extern int g_cFactoryDataEntries;


//Component creation function
class CUnknown;
typedef HRESULT(*FPCREATEINSTANCE)(IUnknown*, CUnknown**);


//CFactoryData 
//Information CFactory needs to create a component supported by DLL

class CFactoryData
{
public:
	//Class ID for the component
	const CLSID* m_pCLSID;
	//Pointer to the function that creates it
	FPCREATEINSTANCE CreateInstance;

	//name of the component to register in the registry
	const char* m_RegistryName;

	//ProgID
	const char* m_szProgID;

	//Version Independent progID
	const char* m_szVerIndProgID;

	//Helper function for finding the class ID
	BOOL IsClassID(const CLSID& clsid) const
	{

		return (*m_pCLSID == clsid);
	}

};


class CFactory : public IClassFactory
{
public:
	//IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID &iid, void **ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//IClassFactory
	virtual HRESULT	__stdcall CreateInstance(IUnknown* pUnknownOuter, const IID &iid, void **ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);

	//Constructor - pass pointer to data of component to create
	CFactory(const CFactoryData* pFactoryData);

	~CFactory() {}

	//Static CFactoryData support functions

	//DllGetClassObject support
	static HRESULT GetClassObject(const CLSID &clsid, const IID &iid, void **ppv);

	//Helper function for DLLCanUnload Now

	static BOOL IsLocked()
	{
		return (s_cServerLocks > 0);
	}

	static HRESULT RegisterAll();
	static HRESULT UnregisterAll();

	static HRESULT CanUnloadNow();


	LONG m_cRef;

	//pointer to information about class this factory creates	
	const CFactoryData* m_pFactoryData;

	//Count of locks
	static LONG s_cServerLocks;

	//Module handle
	static HMODULE s_hModule;
};

#endif