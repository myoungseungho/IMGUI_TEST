#include "Base.h"

CBase::CBase()
{
}

_uint CBase::AddRef()
{	
	return ++m_iRefCnt;
}

_uint CBase::Release()
{
	if (0 == m_iRefCnt)
	{
		Free();		

		int a = 10;

		/* �����Ѵ�. */
		delete this;

		m_Died = true;

		return 0;
	}
	else
	{		
		return m_iRefCnt--;
	}
}
