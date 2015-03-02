#include "StdAfx.h"
#include "CriticalSectionLocker.h"

namespace tools
{
namespace threading
{
	CCriticalSectionLocker::CCriticalSectionLocker(CCriticalSection& cs)
		: _cs(cs)
	{
		_cs.Lock();
	}

	CCriticalSectionLocker::~CCriticalSectionLocker(void)
	{
		_cs.Unlock();
	}
}
}


