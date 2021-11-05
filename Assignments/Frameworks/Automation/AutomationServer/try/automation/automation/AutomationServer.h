#ifndef __AUTOMATIONSERVER_H__
#define __AUTOMATIONSERVER_H__

class IMyMath : public IDispatch
{
public:
	//pure virtual
	virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int*) = 0;
	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*) = 0;
};

//CLSID of MyMath Component
// {C5821FAB-506B-462E-A1B2-4C47138263E9}
const CLSID CLSID_MyMath = { 0xc5821fab, 0x506b, 0x462e, 0xa1, 0xb2, 0x4c, 0x47, 0x13, 0x82, 0x63, 0xe9 };


// {8FB29E18-2549-4723-8DF7-315285EB2A01}
	
//interface ID for ISum
const IID IID_IMyMath = { 0x8fb29e18, 0x2549, 0x4723, 0x8d, 0xf7, 0x31, 0x52, 0x85, 0xeb, 0x2a, 0x1};







#endif