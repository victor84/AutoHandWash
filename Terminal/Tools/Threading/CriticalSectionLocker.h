#pragma once

namespace tools
{
namespace threading
{
/************************************************************************
CCriticalSectionLocker
����� ��� ��������������� ������ Lock() � Unlock()
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


