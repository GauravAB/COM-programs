//This should be a DLL 
#include"faststring.h"
#include "faststringitf.h"

using std::cout;
using std::endl;

FastString::FastString(const char *psz):m_cch(0),m_psz(NULL	)
{
	m_cch = strlen(psz);
	m_psz = psz;
}

FastString::~FastString()
{
	delete m_pThis;
}

FastString::Length(void)const 
{
	return return m_cch;
}

int FastString::Find(const char*psz) const
{
	//to do
}

void FastString::Delete(void)
{
	delete this;
}

void* FastString::Dynamic_Cast( const char *pszType)

{

}

FastString* CreateFastString(const char *psz)
{
	return new FastString(psz);
}










