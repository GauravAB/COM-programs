#ifndef __IFACE_IN_H__
#define __IFACE_IN_H__

interface IMultiply : public IUnknown
{
	virtual void __stdcall MultiplicationOfTwoNumbers( int,int,int*) = 0;
};

interface IDivide : public IUnknown
{
	virtual void __stdcall DivisionOfTwoNumbers(int,int,int*) = 0;
};



//component ID
// {5AAD8F9E-AE57-4131-8027-B38BBACDCF3F}
const CLSID CLSID_CMultiplyDivide = { 0x5aad8f9e, 0xae57, 0x4131, 0x80, 0x27, 0xb3, 0x8b, 0xba, 0xcd, 0xcf, 0x3f };


//Interface IDs
// {0AD886CD-4617-4158-BFF6-E31951000909}
const IID IID_IMultiply = { 0xad886cd, 0x4617, 0x4158, 0xbf, 0xf6, 0xe3, 0x19, 0x51, 0x0, 0x9, 0x9 };
// {531059BB-C5E4-4152-ADCA-A61C507804A3}
const IID IID_IDivide = { 0x531059bb, 0xc5e4, 0x4152, 0xad, 0xca, 0xa6, 0x1c, 0x50, 0x78, 0x4, 0xa3 };


#endif