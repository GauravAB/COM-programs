#ifndef __CONTAINOUTER_H__
#define __CONTAINOUTER_H__

interface ISum : public IUnknown
{
	virtual HRESULT __stdcall additionOfTwoIntegers(int, int, int*) = 0;
};

interface ISub : public IUnknown
{
	virtual HRESULT __stdcall subtractionOfTwoIntegers(int, int, int*) = 0;
};

interface IMultiplication : public IUnknown
{
	virtual HRESULT __stdcall multiplicationOfTwoIntegers(int, int, int*) = 0;
};

interface IDivision : public IUnknown
{
	virtual HRESULT __stdcall divisionOfTwoIntegers(int, int, int*) = 0;
};

// {DFFD0686-23CD-4169-BE34-F16817539EFF}
CLSID CLSID_CSumSubtract = { 0xdffd0686, 0x23cd, 0x4169, 0xbe, 0x34, 0xf1, 0x68, 0x17, 0x53, 0x9e, 0xff };

// {B6C7081E-BEF4-42B5-BB42-CED303CF2290}
IID IID_ISum = { 0xb6c7081e, 0xbef4, 0x42b5, 0xbb, 0x42, 0xce, 0xd3, 0x3, 0xcf, 0x22, 0x90 };

// {A4221161-C9CD-4BD4-B92E-81C99B682971}
IID IID_ISub = { 0xa4221161, 0xc9cd, 0x4bd4, 0xb9, 0x2e, 0x81, 0xc9, 0x9b, 0x68, 0x29, 0x71 };


// {91CE13C0-2E59-496F-B721-7F3874C599AB}
CLSID CLSID_CMulDiv = { 0x91ce13c0, 0x2e59, 0x496f, 0xb7, 0x21, 0x7f, 0x38, 0x74, 0xc5, 0x99, 0xab };

// {83DCDDE7-F519-4CC5-98BB-D2B0B16E514F}
IID IID_IMul = { 0x83dcdde7, 0xf519, 0x4cc5, 0x98, 0xbb, 0xd2, 0xb0, 0xb1, 0x6e, 0x51, 0x4f };

// {A0CE91B6-1AC5-4C40-80EB-4546847A6146}
IID IID_IDiv = { 0xa0ce91b6, 0x1ac5, 0x4c40, 0x80, 0xeb, 0x45, 0x46, 0x84, 0x7a, 0x61, 0x46 };

#endif
