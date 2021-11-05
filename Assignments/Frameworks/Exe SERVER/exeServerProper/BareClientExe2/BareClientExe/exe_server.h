#ifndef __EXE_SERVER_H_
#define __EXE_SERVER_H_


class ITrigo : public IUnknown
{
public:
	virtual HRESULT __stdcall getSine(float angle, float* result) =0 ;
	virtual HRESULT __stdcall getCosine(float angle, float* result)=0;
	virtual HRESULT __stdcall getTangent(float angle, float* result)=0;
}; 

// {4569F083-8A80-419B-BEAD-9569989AF5F6}
const CLSID CLSID_CTrigo = { 0x4569f083, 0x8a80, 0x419b, 0xbe, 0xad, 0x95, 0x69, 0x98, 0x9a, 0xf5, 0xf6};
// {ECDDED6A-7F44-4A00-95BB-FB134B3793DA}
const IID IID_ITrigo = { 0xecdded6a, 0x7f44, 0x4a00, 0x95, 0xbb, 0xfb, 0x13, 0x4b, 0x37, 0x93, 0xda };

	



#endif