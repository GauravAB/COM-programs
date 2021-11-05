#ifndef _MONIKER_H_
#define _MONIKER_H_

class IRandomNumber : public IUnknown
{
	//Odd number specific method declaration'
public:
	virtual HRESULT __stdcall GetNextRandomNumber(int*) = 0;
	virtual HRESULT __stdcall changeSeed(int) = 0;
};

class IRandomNumberFactory : public IUnknown
{
public:
	//IRandomNumberFactory specific method declarations
	virtual HRESULT __stdcall SetSeed(int, IRandomNumber **) = 0;
};

// {468A69F0-8066-43BB-A716-0A56CF948AF6}
const CLSID CLSID_RandomNumber = { 0x468a69f0, 0x8066, 0x43bb, 0xa7, 0x16, 0xa, 0x56, 0xcf, 0x94, 0x8a, 0xf6};

// {2D6C5101-BEF8-4A97-9D9F-F1E9D61B6DD4}
const IID IID_IRandomNumber = { 0x2d6c5101, 0xbef8, 0x4a97, 0x9d, 0x9f, 0xf1, 0xe9, 0xd6, 0x1b, 0x6d, 0xd4};

// {644B2CE0-50C2-4738-9BA4-B31208132B5D}
const IID IID_IRandomNumberFactory = { 0x644b2ce0, 0x50c2, 0x4738, 0x9b, 0xa4, 0xb3, 0x12, 0x8, 0x13, 0x2b, 0x5d};


#endif