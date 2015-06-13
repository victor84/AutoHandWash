#pragma once
#include "state.h"
#include "TraceError.h"

namespace logic
{
class CPaidIdleState : public IState
{
	tools::logging::CTraceError* _tr_error;

	// ��������� ����������
	tag_device_settings _device_settings;

	// ������, ������������� ������ ������� ��� ����������� ������
	Concurrency::timer<int32_t>* _timer;

	// ���������� ��������
	Concurrency::call<int32_t> _on_timer_call;

	// ���������� ����� � ��������
	int16_t _time_left;

	// ���������� ������ (� ��������)
	int32_t _balance_of_money;

	// ��������� ������� (� ��������)
	int16_t _current_cost;

	// ���������� ��������
	void on_timer(int32_t);

	// ���������� ������� ����� � �������
	void calc_time_and_money();

	// ���������� ������� ����� � ������������ � ���������� ��������
	void calc_money_balance_by_time_left();

	// ���������� ������
	void stop_timer();

public:
	CPaidIdleState(CLogicAbstract& logic);
	virtual ~CPaidIdleState();

	virtual void refilled_cache() final;

	virtual void service_button_press(e_service_name service_name) final;

	virtual void stop_button_press() final;

	virtual void time_out() final;

	virtual void out_of_money() final;

	virtual void device_confirm(device_exchange::e_command_from_pc command) final;

	virtual void device_error(logic_structures::e_device_error_code code) final;

	virtual void activate() override;

	virtual void deactivate() override;

};
}



