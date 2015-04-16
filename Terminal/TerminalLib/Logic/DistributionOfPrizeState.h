#pragma once
#include "state.h"
#include "CorrespondSettings.h"

namespace logic
{
class CDistributionOfPrizeState : public IState
{
public:
	CDistributionOfPrizeState(CLogicAbstract& logic);
	virtual ~CDistributionOfPrizeState();

	virtual void refilled_cache() final;

	virtual void service_button_press(e_service_name service_name) final;

	virtual void stop_button_press() final;

	virtual void time_out() final;

	virtual void out_of_money() final;

	virtual void device_confirm() final;

	virtual void device_error(logic_structures::e_device_error_code code) final;

	// выдать приз
	void distribute(uint16_t size);
};
}



