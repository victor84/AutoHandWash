#pragma once
#include "state.h"
#include "CorrespondSettings.h"

namespace logic
{
class CExecutingServiceState : public IState
{
	// модуль соответствий услуг, кнопок и клапанов
	 logic_settings::CCorrespondSettings _correspond_settings;

public:
	CExecutingServiceState(CLogicAbstract& logic);
	virtual ~CExecutingServiceState();

	virtual void refilled_cache(uint16_t cache) final;

	virtual void service_button_press(e_service_name sevice_name) final;

	virtual void stop_button_press() final;

	virtual void time_out() final;

	virtual void out_of_money() final;

};
}



