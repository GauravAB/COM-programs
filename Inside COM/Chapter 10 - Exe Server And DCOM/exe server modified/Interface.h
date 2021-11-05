#ifndef __IFACE_H
#define __IFACE_H

interface ISum : public IUnknown
{
	virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int *) = 0;
};

interface ISubtract : public IUnknown
{
	virtual  HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*) = 0;
};

// {7E06580B-35EB-424E-BB0B-B6426F0E998E}
const CLSID CLSID_CSumSubtract = { 0x7e06580b, 0x35eb, 0x424e, 0xbb, 0xb, 0xb6, 0x42, 0x6f, 0xe, 0x99, 0x8e };

// {727E87DA-D534-47F2-B962-1FC887D4AB10}
const IID IID_ISum = { 0x727e87da, 0xd534, 0x47f2, 0xb9, 0x62, 0x1f, 0xc8, 0x87, 0xd4, 0xab, 0x10 };

// {0F448DFF-C4B8-4061-A340-0D0B4F946F2A}
const IID IID_ISubtract = { 0xf448dff, 0xc4b8, 0x4061, 0xa3, 0x40, 0xd, 0xb, 0x4f, 0x94, 0x6f, 0x2a };


#endif