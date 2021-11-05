#pragma once

interface ISum : public IUnknown
{
	virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int*) = 0;
};

interface ISubtract : public IUnknown
{
	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*) = 0;
};

// {07255CAA-D856-4167-B8B8-BC5A9A4A0CCB}
CLSID CLSID_CSumSubtract = { 0x7255caa, 0xd856, 0x4167, 0xb8, 0xb8, 0xbc, 0x5a, 0x9a, 0x4a, 0xc, 0xcb};

// {13B5385F-F303-43F0-9792-CD428625BACD}
IID IID_ISum = { 0x13b5385f, 0xf303, 0x43f0, 0x97, 0x92, 0xcd, 0x42, 0x86, 0x25, 0xba, 0xcd};

// {E72555D2-3C2D-4953-82DF-9AD9790F7BA7}
IID IID_ISubtract = { 0xe72555d2, 0x3c2d, 0x4953, 0x82, 0xdf, 0x9a, 0xd9, 0x79, 0xf, 0x7b, 0xa7};


