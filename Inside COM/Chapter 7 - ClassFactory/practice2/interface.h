#ifndef _IFACE_H
#define _IFACE_H

#include <windows.h>

interface ISum : IUnknown
{
	virtual void __stdcall SumOfTwoIntegers(int,int,int*) = 0;
};

interface ISubtract :IUnknown
{
	virtual void __stdcall SubtractionOfTwoIntegers(int ,int,int*) = 0;
};




// {4E509373-2852-43BA-BF07-EB6D78E4E8CC}
extern "C" const IID IID_ISum = {0x4e509373, 0x2852, 0x43ba, 0xbf, 0x7, 0xeb, 0x6d, 0x78, 0xe4, 0xe8, 0xcc};


// {27D45891-F598-45EA-A480-F70D0B5477C4}
extern "C" const IID IID_ISubtract ={0x27d45891, 0xf598, 0x45ea, 0xa4, 0x80, 0xf7, 0xd, 0xb, 0x54, 0x77, 0xc4};


// {D8C2CBF5-6140-4D88-9D71-FF690ED1BB98}
extern "C" const CLSID CLSID_CMath ={0xd8c2cbf5, 0x6140, 0x4d88, 0x9d, 0x71, 0xff, 0x69, 0xe, 0xd1, 0xbb, 0x98};






#endif