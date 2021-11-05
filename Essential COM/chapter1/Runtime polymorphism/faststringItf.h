#ifndef _FASTSTRINGITF_H
#define _FASTSTRINGITF_H

class IFastString
{
	class FastString;	//introduce name of implementing class
	FastString *m_pThis;	//opaque pointer

public:
	IFastString(const char* psz);
	~IFastString(void);

	virtual int Length(void)const = 0;	//returns number of characters
	virtual int Find(const char *psz) const = 0;	//returns offset
	virtual void Delete(void) = 0;
};

extern "C" IFastString *CreateFastString(const char *psz);



#endif