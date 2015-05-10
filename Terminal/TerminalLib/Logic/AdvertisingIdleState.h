#pragma once
#include "state.h"
#include "TraceError.h"
#include "CorrespondSettings.h"
#include "CommonSettings.h"

namespace logic
{
class CAdvertisingIdleState : public IState
{
	tools::logging::CTraceError* _tr_error;

	// модуль соответствий услуг, кнопок и клапанов
	logic_settings::CCorrespondSettings& _correspond_settings;

	// модуль общих настроек терминала
	logic_settings::CCommonSettings& _common_settings;

#pragma region ѕоказ рекламы
	// таймер перед показом рекламы
	Concurrency::timer<int32_t>* _idle_timer;

	// вызываетс€ при срабатывании таймера перед показом рекламы
	Concurrency::call<int32_t> _on_idle_timer_call;

	// таймер перед показом рекламы сработал
	void on_idle_timer(uint32_t);

	// остановка таймера перед показом рекламы
	void stop_idle_timer();
#pragma endregion

#pragma region ѕроверка нажати€ 2-х кнопок дл€ показа счЄтчиков
	// таймер проверки нажати€ 2-х кнопок
	Concurrency::timer<int32_t>* _counters_timer;

	// вызываетс€ при срабатывании таймера проверки нажати€ 2-х кнопок
	Concurrency::call<int32_t> _on_counters_timer_call;

	// таймер проверки нажати€ 2-х кнопок сработал
	void on_counters_timer(uint32_t);

	// остановка таймера проверки нажати€ 2-х кнопок сработал
	void stop_counters_timer();
#pragma endregion

	// прочитать и записать настройки по-умолчанию стоимости услуг
	bool read_services_cost(tag_device_settings& settings);

	// чтение настроек терминала
	bool read_terminal_settings(tag_device_settings& settings);

public:
	CAdvertisingIdleState(CLogicAbstract& logic, 
						  logic_settings::CCorrespondSettings& correspond_settings,
						  logic_settings::CCommonSettings& common_settings);
	virtual ~CAdvertisingIdleState();

	virtual void refilled_cache() final;

	virtual void service_button_press(e_service_name service_name);

	virtual void stop_button_press() final;

	virtual void time_out() final;

	virtual void out_of_money() final;

	virtual void device_confirm() final;

	virtual void device_error(logic_structures::e_device_error_code code) final;

	virtual void activate() final;

	void buttons_state(const logic_structures::tag_buttons_state& buttons_state);

	virtual void deactivate() final;

};
}



