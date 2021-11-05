#ifndef _MONIKER_H_
#define _MINIKER_H_

class IOddNumber : public IUnknown
{
	//Odd number specific method declaration'

	virtual HRESULT __stdcall GetNextOddNumber(int*) = 0;
};

class IOddNumberFactory : public IUnknown
{
public:
	//IOddNumberFactory specific method declarations
	virtual HRESULT __stdcall SetFirstOddNumber(int, IOddNumber **) = 0;
};


// {D8A8F4F1-2091-477F-BD02-8D79857CFA0E}
const CLSID CLSID_OddNumber = { 0xd8a8f4f1, 0x2091, 0x477f, 0xbd, 0x2, 0x8d, 0x79, 0x85, 0x7c, 0xfa, 0xe};

// {CABB6F39-BEBF-4A38-9CF1-E4EDE0C487B7}
const IID IID_IOddNumber = { 0xcabb6f39, 0xbebf, 0x4a38, 0x9c, 0xf1, 0xe4, 0xed, 0xe0, 0xc4, 0x87, 0xb7};


// {6E42E005-114A-4C86-AC1A-96B4CCA9E3E9}
const IID IID_IOddNumberFactory = { 0x6e42e005, 0x114a, 0x4c86, 0xac, 0x1a, 0x96, 0xb4, 0xcc, 0xa9, 0xe3, 0xe9};











#endif