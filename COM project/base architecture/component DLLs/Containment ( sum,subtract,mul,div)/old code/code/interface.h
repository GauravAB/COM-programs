#ifndef __IFACE_OUT_H__
#define __IFACE_OUT_H__

interface ISum : public IUnknown
{
	virtual void __stdcall SumOfTwoNumbers( int,int,int*) = 0;
};

interface ISubtract : public IUnknown
{
	virtual void __stdcall SubtractionOfTwoNumbers(int,int,int*) = 0;
};


// {2872E2C4-9232-41BC-8117-650BD42757C0}
//component ID
const CLSID CLSID_CSumSubtract = { 0x2872e2c4, 0x9232, 0x41bc, 0x81, 0x17, 0x65, 0xb, 0xd4, 0x27, 0x57, 0xc0 };

//Interface IDs
// {492C40D4-5036-44D4-8371-3EB7270523F7}
const IID IID_ISum = { 0x492c40d4, 0x5036, 0x44d4, 0x83, 0x71, 0x3e, 0xb7, 0x27, 0x5, 0x23, 0xf7 };
// {FCFD2806-0746-4F72-92E4-93E6AD682DFB}
const IID IID_ISubtract = { 0x556726bb, 0xae3d, 0x439d, 0xb4, 0x9, 0x61, 0x9c, 0x5a, 0xdc, 0x25, 0xae };




#endif