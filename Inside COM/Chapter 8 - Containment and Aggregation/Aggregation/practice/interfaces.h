
interface ISum : IUnknown
{
	virtual int __stdcall SumOfTwoIntegers(int,int) = 0 ;
};


interface ISubtract : IUnknown
{
	virtual int __stdcall SubtractionOfTwoIntegers(int,int) = 0;
};



// {EBDA944E-CF32-454E-98FB-06EFFE7CCBCD}
extern "C" const IID IID_ISum = {0xebda944e, 0xcf32, 0x454e, 0x98, 0xfb, 0x6, 0xef, 0xfe, 0x7c, 0xcb, 0xcd};

// {28E0F5A8-E287-4763-82CB-4E4FB86A3039}
extern "C" const IID IID_ISubtract = {0x28e0f5a8, 0xe287, 0x4763, 0x82, 0xcb, 0x4e, 0x4f, 0xb8, 0x6a, 0x30, 0x39};

// {46D1F49E-9745-4A74-AEA8-44EE4A32A515}
extern "C" const CLSID CLSID_CMath = {0x46d1f49e, 0x9745, 0x4a74, 0xae, 0xa8, 0x44, 0xee, 0x4a, 0x32, 0xa5, 0x15};

// {47DB8378-4816-4E85-9660-7D98316E45CC}
extern "C" const CLSID CLSID_CMathMore = {0x47db8378, 0x4816, 0x4e85, 0x96, 0x60, 0x7d, 0x98, 0x31, 0x6e, 0x45, 0xcc};

