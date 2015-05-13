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

	// ������ ������������ �����, ������ � ��������
	logic_settings::CCorrespondSettings& _correspond_settings;

	// ������ ����� �������� ���������
	logic_settings::CCommonSettings& _common_settings;

#pragma region ����� �������
	// ������ ����� ������� �������
	Concurrency::timer<int32_t>* _idle_timer;

	// ���������� ��� ������������ ������� ����� ������� �������
	Concurrency::call<int32_t> _on_idle_timer_call;

	// ������ ����� ������� ������� ��������
	void on_idle_timer(uint32_t);

	// ��������� ������� ����� ������� �������
	void stop_idle_timer();
#pragma endregion

#pragma region �������� ������� 2-� ������ ��� ������ ���������
	// ������ �������� ������� 2-� ������
	Concurrency::timer<int32_t>* _counters_timer;

	// ���������� ��� ������������ ������� �������� ������� 2-� ������
	Concurrency::call<int32_t> _on_counters_timer_call;

	// ������ �������� ������� 2-� ������ ��������
	void on_counters_timer(uint32_t);

	// ��������� ������� �������� ������� 2-� ������ ��������
	void stop_counters_timer();
#pragma endregion

	// ��������� � �������� ��������� ��-��������� ��������� �����
	bool read_services_cost(tag_device_settings& settings);

	// ������ �������� ���������
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



