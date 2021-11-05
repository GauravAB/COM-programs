#ifndef __IFACEOUT_H__
#define __IFACEOUT_H__

interface ISum : public IUnknown
{
	virtual void __stdcall SumOfTwoNumbers( int,int,int*) = 0;
};

interface ISubtract : public IUnknown
{
	virtual void __stdcall SubtractionOfTwoNumbers(int,int,int*) = 0;
};


// {3A37BBDC-058B-4B89-9320-0B1EEBD017D8}
//component ID
const CLSID CLSID_CSumSubtract = {0x3a37bbdc, 0x58b, 0x4b89, 0x93, 0x20, 0xb, 0x1e, 0xeb, 0xd0, 0x17, 0xd8};

//Interface IDs
// {583EE849-8A50-4720-8E42-2E32ECDD39BD}
const IID IID_ISum = {0x583ee849, 0x8a50, 0x4720, 0x8e, 0x42, 0x2e, 0x32, 0xec, 0xdd, 0x39, 0xbd};
// {FCFD2806-0746-4F72-92E4-93E6AD682DFB}
const IID IID_ISubtract = {0xfcfd2806, 0x746, 0x4f72, 0x92, 0xe4, 0x93, 0xe6, 0xad, 0x68, 0x2d, 0xfb};

#endif