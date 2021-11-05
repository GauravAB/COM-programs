//This should be a DLL 
#include"faststring.h"
#include "faststringitf.h"

using std::cout;
using std::endl;

IFastString::IFastString(const char *psz):m_pThis(new FastString(psz))
{
	if(m_pThis == NULL)
	{
		cout <<"Out of memory"<<endl;
	}
}

IFastString::~IFastString()
{
	delete m_pThis;
}

IFastString::Length(void)const 
{
	return m_pThis->Length();
}

int IFastString::Find(const char*psz) const
{
	m_pThis->Find(psz);
}

void IFastString::Delete(void)
{
	delete this;
}

IFastString* CreateFastString(const char *psz)
{
	return new FastString(psz);
}







