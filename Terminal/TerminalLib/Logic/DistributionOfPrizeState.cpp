#include "stdafx.h"
#include "DistributionOfPrizeState.h"
#include "data_from_pc.h"


logic::CDistributionOfPrizeState::CDistributionOfPrizeState(CLogicAbstract& logic)
															: IState(logic, e_state::distribution_of_prize)
															, _rest_coins(0)
															, _hopper_empty(false)
{
}

logic::CDistributionOfPrizeState::~CDistributionOfPrizeState()
{
}

void logic::CDistributionOfPrizeState::refilled_cache()
{

}

void logic::CDistributionOfPrizeState::service_button_press(e_service_name service_name)
{
	if (true == _hopper_empty)
	{
		_hopper_empty = false;
		_logic.set_state(e_state::advertising_idle);
	}
}

void logic::CDistributionOfPrizeState::stop_button_press()
{
	if (true == _hopper_empty)
	{
		_logic.send_issue_coins_packet_to_device(_rest_coins);
	}
}

void logic::CDistributionOfPrizeState::time_out()
{

}

void logic::CDistributionOfPrizeState::out_of_money()
{

}

void logic::CDistributionOfPrizeState::device_confirm(device_exchange::e_command_from_pc command)
{

}

void logic::CDistributionOfPrizeState::device_error(logic_structures::e_device_error_code code)
{
	if (logic_structures::e_device_error_code::empty_hopper == code)
	{
		_hopper_empty = true;
		_logic.on_empty_hopper();
	}
}

void logic::CDistributionOfPrizeState::distribute(uint16_t size)
{
	byte count = static_cast<byte>(size / 10);

	_rest_coins = count;
	
	_logic.send_issue_coins_packet_to_device(_rest_coins);
}

void logic::CDistributionOfPrizeState::coin_issued(byte balance)
{
	_logic.coin_issued(balance);

	_rest_coins = balance;

	if (0 == balance)
	{
		_logic.send_log_to_server(server_exchange::e_log_record_type::message, _T("Приз успешно выдан."));
		_logic.set_state(e_state::advertising_idle);
	}
}

void logic::CDistributionOfPrizeState::activate()
{
	
}

void logic::CDistributionOfPrizeState::deactivate()
{

}
