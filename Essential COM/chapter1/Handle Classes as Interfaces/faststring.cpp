//This should be a DLL 
#include"faststring.h"
#include "faststringitf.h"

using std::cout;
using std::endl;

FastStringItf::FastStringItf(const char *psz):m_pThis(new FastString(psz))
{
	if(m_pThis == NULL)
	{
		cout <<"Out of memory"<<endl;
	}
}

FastStringItf::~FastStringItf()
{
	delete m_pThis;
}

FastStringItf::LengthItf(void)const 
{
	return m_pThis->Length();
}

int FastStringItf::Find(const char*psz) const
{
	m_pThis->Find(psz);
}






