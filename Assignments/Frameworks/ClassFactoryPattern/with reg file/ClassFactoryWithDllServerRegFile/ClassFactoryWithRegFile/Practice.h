
class ISum : public IUnknown
{
	public: 
	
		//pure virtual function
		virtual HRERSULT __stdcall SumOfTwoIntegers(int , int , int *) = 0;

};

class ISubtract : public IUnknown
{
	public:

		//pure virtual function
		virtual HRESULT __stdcall SubtractionOfTwoIntegers(int,int,int*) = 0;

}


const CLSID CLSID_SumSubtract = {}

const IID IID_ISum = {}

const IID IID_ISubtract = {}

