#ifndef _FASTSTRINGITF_H
#define _FASTSTRINGITF_H

class FaststringItf
{
	class FastString;	//introduce name of implementing class
	FastString *m_pThis;	//opaque pointer

public:
	FastStringItf(const char* psz);
	~FastStringItf(void);

	int Length(void)const;	//returns number of characters
	int Find(const char *psz) const;	//returns offset
};

#endif