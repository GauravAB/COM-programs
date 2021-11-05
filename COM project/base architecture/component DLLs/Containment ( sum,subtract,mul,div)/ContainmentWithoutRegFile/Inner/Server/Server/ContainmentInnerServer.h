#ifndef __CONTAININNER_H__
#define __CONTAININNER_H__

interface IMultiplication : public IUnknown
{
	virtual HRESULT __stdcall multiplicationOfTwoIntegers(int, int, int*) = 0;
};

interface IDivision : public IUnknown
{
	virtual HRESULT __stdcall divisionOfTwoIntegers(int, int, int*) = 0;
};


// {91CE13C0-2E59-496F-B721-7F3874C599AB}
CLSID CLSID_CMulDiv = { 0x91ce13c0, 0x2e59, 0x496f, 0xb7, 0x21, 0x7f, 0x38, 0x74, 0xc5, 0x99, 0xab};

// {83DCDDE7-F519-4CC5-98BB-D2B0B16E514F}
IID IID_IMul = { 0x83dcdde7, 0xf519, 0x4cc5, 0x98, 0xbb, 0xd2, 0xb0, 0xb1, 0x6e, 0x51, 0x4f};

// {A0CE91B6-1AC5-4C40-80EB-4546847A6146}
IID IID_IDiv = {0xa0ce91b6, 0x1ac5, 0x4c40, 0x80, 0xeb, 0x45, 0x46, 0x84, 0x7a, 0x61, 0x46 };
#endif
