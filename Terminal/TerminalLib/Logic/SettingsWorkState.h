#pragma once
#include "state.h"

namespace logic
{
class CSettingsWorkState : public IState
{
public:
	CSettingsWorkState(CLogicAbstract& logic);
	virtual ~CSettingsWorkState();
};
}



