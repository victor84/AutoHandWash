#include "stdafx.h"
#include "DistributionOfPrizeState.h"
#include "data_from_pc.h"


logic::CDistributionOfPrizeState::CDistributionOfPrizeState(CLogicAbstract& logic)
															: IState(logic, e_state::distribution_of_prize)
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

}

void logic::CDistributionOfPrizeState::stop_button_press()
{

}

void logic::CDistributionOfPrizeState::time_out()
{

}

void logic::CDistributionOfPrizeState::out_of_money()
{

}

void logic::CDistributionOfPrizeState::device_confirm()
{

}

void logic::CDistributionOfPrizeState::device_error(logic_structures::e_device_error_code code)
{

}

void logic::CDistributionOfPrizeState::distribute(uint16_t size)
{
	byte count = static_cast<byte>(size / 10);
	
	_logic.send_issue_coins_packet_to_device(count);
}
