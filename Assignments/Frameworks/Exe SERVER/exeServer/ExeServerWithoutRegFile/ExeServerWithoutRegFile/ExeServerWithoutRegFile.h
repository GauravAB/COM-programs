#ifndef __EXESERVER_H__
#define __EXESERVER_H__



class ITrigoFunctions : public IUnknown
{
public:
	virtual HRESULT __stdcall getSine(float angle , float*result) =0;
	virtual HRESULT __stdcall getCosine(float angle , float *result)=0;
	virtual HRESULT __stdcall getTangent(float angle , float *result)=0;
};

// {1CCC0A45-3C56-4A83-98E0-F8486124A944}
const CLSID CLSID_CTrigoFunctions = { 0x1ccc0a45, 0x3c56, 0x4a83, 0x98, 0xe0, 0xf8, 0x48, 0x61, 0x24, 0xa9, 0x44};

// {B1FFB522-82D0-43FE-95A7-D988FD625E86}
const IID IID_ITrigoFunctions = { 0xb1ffb522, 0x82d0, 0x43fe, 0x95, 0xa7, 0xd9, 0x88, 0xfd, 0x62, 0x5e, 0x86};

#endif

