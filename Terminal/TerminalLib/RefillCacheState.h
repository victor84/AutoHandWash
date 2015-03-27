#pragma once
#include "state.h"

namespace logic
{
class CRefillCacheState : public IState
{
	int16_t _cache;

public:
	CRefillCacheState();
	virtual ~CRefillCacheState();

	virtual void refilled_cache(uint16_t cache) override;

	virtual void service_button_press(e_service_name sevice_name) override;

	virtual void press_button_press() override;

	virtual void time_out() override;

	virtual void out_of_money() override;

	int16_t get_cache() const
	{
		return _cache;
	}

};
}