#pragma once

namespace tools
{
namespace threading
{
/************************************************************************
CCriticalSectionLocker
класс для автоматического вызова Lock() и Unlock()
MFC CCriticalSection
************************************************************************/

class CCriticalSectionLocker
{
	CCriticalSection& _cs;

public:
	CCriticalSectionLocker(CCriticalSection& cs);
	virtual ~CCriticalSectionLocker(void);
};
}
}


