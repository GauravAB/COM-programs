#ifndef __FINDSTRING_H
#define __FINDSTRING_H
#include <iostream>
#include <cstring>

using std::cout;
using std::endl;

class FastString
{
	const int m_cch;	//cout of characters
	char *m_psz;

public:
	FastString(const char *psz);
	~FastString(void);
	int Length(void)const;	//returns number of characters
	int Find(const char *psz)const ; //returns offset
};


FastString::FastString(const char *psz):m_cch(strlen(psz)),m_psz(new char[m_cch +1])
{
	strcpy(m_psz,psz); //cache the length of the string
}

int FastString::Length(void)const
{
	return m_cch;	//return the cached length
}

int FastString::Find(const char *psz) const
{
	// to code this

	return 1;
}


#endif
