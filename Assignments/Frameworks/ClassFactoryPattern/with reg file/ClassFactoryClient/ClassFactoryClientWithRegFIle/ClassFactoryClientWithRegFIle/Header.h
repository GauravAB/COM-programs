class ISum : public IUnknown
{
public:
	virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int*) = 0; //pure virtual
};

class ISubtract : public IUnknown
{
public:
	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*) = 0; //pure virtual function
};


const CLSID CLSID_SumSubtract = { 0x2c37330c, 0xafd7, 0x4d19, 0xbc, 0x15, 0x23, 0x8f, 0xc5, 0xff, 0xb6, 0xc3};
//CLSID SumSubstract Component {2C37330C-AFD7-4D19-BC15-238FC5FFB6C3}

const IID IID_ISum = { 0x27cbcf18, 0x717a, 0x40e7, 0xa6, 0x94, 0xc0, 0xe6, 0x9, 0xc9, 0xb1, 0x5d };
// {27CBCF18-717A-40E7-A694-C0E609C9B15D}

const IID IID_ISubtract = { 0xb4ba86dd, 0xcd86, 0x449b, 0x93, 0x7, 0x9c, 0x4c, 0x23, 0x1b, 0x8e, 0x2d};
// {B4BA86DD-CD86-449B-9307-9C4C231B8E2D}

