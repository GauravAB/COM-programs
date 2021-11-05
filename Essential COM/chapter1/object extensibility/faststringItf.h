#ifndef _FASTSTRINGITF_H
#define _FASTSTRINGITF_H


class IFastString : public IExtensibleObject
{
public:
	virtual int Length(void)const = 0;	//returns number of characters
	virtual int Find(const char *psz) const = 0;	//returns offset
};


class IExtensibleObject
{
public:
	virtual void* Dynamic_Cast(const char* pszType) = 0;
	virtual void Delete(void) = 0;
};

class IPersistentObject : public IExtensibleObject
{
public:
	virtual bool Load(const char *pszFileName) = 0;
	virtual bool Save(const char *pszFileName) = 0;
};




extern "C" IFastString *CreateFastString(const char *psz);



#endif