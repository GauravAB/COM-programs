#ifndef __IFACEINN_H__
#define __IFACEINN_H__

interface IMultiply : public IUnknown
{
	virtual void __stdcall MultiplicationOfTwoNumbers(int, int, int*) = 0;
};

interface IDivide : public IUnknown
{
	virtual void __stdcall DivisionOfTwoNumbers(int, int, int*) = 0;
};


//component ID
// {BB893F77-FF1C-4148-970A-17F487502E67}
const CLSID CLSID_CMultiplyDivide = { 0xbb893f77, 0xff1c, 0x4148, 0x97, 0xa, 0x17, 0xf4, 0x87, 0x50, 0x2e, 0x67 };

//Interface IDs
// {ADAFBE46-72E8-49D6-8ED7-141C82E5F859}
const IID IID_IMultiply = { 0xadafbe46, 0x72e8, 0x49d6, 0x8e, 0xd7, 0x14, 0x1c, 0x82, 0xe5, 0xf8, 0x59};
// {1C835DF0-81B7-4C7B-901A-A79E9DE81B62}
const IID IID_IDivide = { 0x1c835df0, 0x81b7, 0x4c7b, 0x90, 0x1a, 0xa7, 0x9e, 0x9d, 0xe8, 0x1b, 0x62};


#endif