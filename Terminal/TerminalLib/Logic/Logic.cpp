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

	_device_interact.Start();
	_server_interact.Start();

	_init_state = tools::e_init_state::was_init;
	return _init_state;
}

void logic::CLogic::on_connected_to_server()
{
	
}

void logic::CLogic::on_disconnected_from_derver()
{

}

logic::CLogic::CLogic()
	: _device_interact(_common_settings, _packets_from_device, _packets_to_device)
	, _server_interact(_common_settings, _packets_from_server, _packets_to_server, 
						std::bind(std::mem_fn(&CLogic::on_connected_to_server), this), 
						std::bind(std::mem_fn(&CLogic::on_disconnected_from_derver), this))
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

logic::CLogic::~CLogic()
{
}

bool logic::CLogic::Start()
{
	if (tools::e_init_state::was_init == _init_state)
		return true;

	return tools::e_init_state::was_init == init();
}

void logic::CLogic::Stop()
{
	if (tools::e_init_state::not_init == _init_state)
		return;

	_device_interact.Stop();
	_server_interact.Stop();

	_work_loop_status = tools::e_work_loop_status::stop;

	if (_this_thread.joinable())
		_this_thread.join();

	_init_state = tools::e_init_state::not_init;
}
