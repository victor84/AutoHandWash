#include "stdafx.h"
#include "RefillCacheState.h"

logic::CRefillCacheState::CRefillCacheState()
{
}


logic::CRefillCacheState::~CRefillCacheState()
{
}

void logic::CRefillCacheState::refilled_cache(uint16_t cache)
{
	_cache += static_cast<int16_t>(cache);
}

void logic::CRefillCacheState::service_button_press(e_service_name sevice_name)
{

}

void logic::CRefillCacheState::press_button_press()
{

}

void logic::CRefillCacheState::time_out()
{

}

void logic::CRefillCacheState::out_of_money()
{

}
