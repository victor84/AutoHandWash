#pragma once
#include "state.h"
#include "CorrespondSettings.h"

namespace logic
{
class CDistributionOfPrizeState : public IState
{
	byte _rest_coins;
	bool _hopper_empty;

	std::wstringstream _str_str;

	tools::logging::CTraceError* _tr_error;

public:
	CDistributionOfPrizeState(CLogicAbstract& logic);
	virtual ~CDistributionOfPrizeState();

	virtual void refilled_cache() final;

	virtual void service_button_press(e_service_name service_name) final;

	virtual void stop_button_press() final;

	virtual void time_out() final;

	virtual void out_of_money() final;

	virtual void device_confirm(device_exchange::e_command_from_pc command) final;

	virtual void device_error(logic_structures::e_device_error_code code) final;

	// ������ ����
	void distribute(uint16_t size);

	// ������ �������
	void coin_issued(byte balance);

	virtual void activate() override;

	virtual void deactivate() override;

};
}



