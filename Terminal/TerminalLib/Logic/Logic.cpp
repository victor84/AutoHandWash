#include "stdafx.h"
#include "Logic.h"


void logic::CLogic::thread_fn()
{
	while (tools::e_work_loop_status::ok == _work_loop_status)
	{

	}
}

tools::e_init_state logic::CLogic::init()
{
	if (tools::e_init_state::was_init == _init_state)
		return _init_state;

	if (false == _common_settings.ReadSettings())
		return tools::e_init_state::not_init;

	if (false == _correspond_settings.ReadSettings())
		return tools::e_init_state::not_init;

	_init_state = tools::e_init_state::was_init;
	return _init_state;
}

logic::CLogic::CLogic()
	: _device_interact(_packets_to_device, _packets_from_device, _common_settings)
	, _server_interact(_common_settings, _packets_from_server, _packets_to_server)
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

logic::CLogic::~CLogic()
{
}

bool logic::CLogic::Start()
{
	return false;
}

void logic::CLogic::Stop()
{
	_work_loop_status = tools::e_work_loop_status::stop;
}
