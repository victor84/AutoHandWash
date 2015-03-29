#include "stdafx.h"
#include "Logic.h"
#include "AdvertisingIdleState.h"
#include "RefillCacheState.h"
#include "ExecutingServiceState.h"
#include "data_from_device.h"
#include "SettingsWorkState.h"
#include "FreeEdleState.h"
#include "PaidIdleState.h"

void logic::CLogic::fill_states()
{
	_states.clear();

	_states.insert(std::make_pair(e_state::advertising_idle, std::make_shared<CAdvertisingIdleState>(*(dynamic_cast<CLogicAbstract*>(this)))));
	_states.insert(std::make_pair(e_state::refill_cache, std::make_shared<CRefillCacheState>(*(dynamic_cast<CLogicAbstract*>(this)))));
	_states.insert(std::make_pair(e_state::executing_service, std::make_shared<CExecutingServiceState>(*(dynamic_cast<CLogicAbstract*>(this)))));
	_states.insert(std::make_pair(e_state::settings_work, std::make_shared<CSettingsWorkState>(*(dynamic_cast<CLogicAbstract*>(this)))));
	_states.insert(std::make_pair(e_state::free_idle, std::make_shared<CFreeEdleState>(*(dynamic_cast<CLogicAbstract*>(this)))));
	_states.insert(std::make_pair(e_state::paid_idle, std::make_shared<CPaidIdleState>(*(dynamic_cast<CLogicAbstract*>(this)))));

}

void logic::CLogic::thread_fn()
{
	_current_state = get_state(e_state::settings_work);
	CSettingsWorkState* sws = dynamic_cast<CSettingsWorkState*>(_current_state.get());
	sws->read_settings();

	while (tools::e_work_loop_status::ok == _work_loop_status)
	{
		process_messages_from_device();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
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

	_this_thread = std::thread(&CLogic::thread_fn, this);

	_init_state = tools::e_init_state::was_init;
	return _init_state;
}

void logic::CLogic::on_connected_to_server()
{
	send_identification_packet();
}

void logic::CLogic::on_disconnected_from_derver()
{

}

void logic::CLogic::send_identification_packet()
{
	server_exchange::tag_identification_packet identification_packet;

	std::wstring terminal_name = _common_settings.GetTerminalName();
	std::wstring terminal_group = _common_settings.GetTerminalGroup();

	terminal_name._Copy_s(identification_packet.terminal_name, sizeof(identification_packet.terminal_name), terminal_name.size());
	terminal_group._Copy_s(identification_packet.group_name, sizeof(identification_packet.group_name), terminal_group.size());

	logic_structures::tag_server_logic_packet <server_exchange::tag_identification_packet, server_exchange::e_packet_type::id>* 
		server_logic_packet = new logic_structures::tag_server_logic_packet < server_exchange::tag_identification_packet, server_exchange::e_packet_type::id > (identification_packet);

	std::shared_ptr<logic_structures::tag_base_server_logic_struct> packet = 
		std::shared_ptr<logic_structures::tag_base_server_logic_struct>(server_logic_packet);

	_server_interact.PushFrontToSend(packet);
}

void logic::CLogic::send_settings_packet()
{

}

logic::CLogic::CLogic()
	: _device_interact(_common_settings, _packets_from_device, _packets_to_device)
	, _server_interact(_common_settings, _packets_from_server, _packets_to_server, 
						std::bind(std::mem_fn(&CLogic::on_connected_to_server), this), 
						std::bind(std::mem_fn(&CLogic::on_disconnected_from_derver), this))
{
	_tr_error = tools::logging::CTraceError::get_instance();
	fill_states();
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

	std::this_thread::sleep_for(std::chrono::seconds(2));
}

void logic::CLogic::open_valve(byte number)
{
	std::shared_ptr<logic_structures::tag_open_valve> ov_message = std::make_shared<logic_structures::tag_open_valve>() ;

	ov_message->number = number;

	_packets_to_device.push_back(ov_message);
}

void logic::CLogic::close_valve(byte number)
{
	std::shared_ptr<logic_structures::tag_close_valve> cv_message = std::make_shared<logic_structures::tag_close_valve>();

	cv_message->number = number;

	_packets_to_device.push_back(cv_message);
}

void logic::CLogic::read_eeprom(byte cell_number)
{
	std::shared_ptr<logic_structures::tag_read_eeprom> re_message = std::make_shared<logic_structures::tag_read_eeprom>();

	re_message->cell_number = cell_number;

	_packets_to_device.push_back(re_message);
}

void logic::CLogic::write_eeprom(byte cell_number, uint32_t value)
{
	std::shared_ptr<logic_structures::tag_write_eeprom> we_message = std::make_shared<logic_structures::tag_write_eeprom>();

	we_message->cell_number = cell_number;
	we_message->value = value;

	_packets_to_device.push_back(we_message);
}

void logic::CLogic::time_and_money(int16_t time, int16_t money)
{
	_str_str.str(std::wstring());

	_str_str << _T("Осталось времени: ") << time << _T(" сек.") << _T("\r\n") <<
		_T("Осталось денег на счёте: ") << static_cast<float>(money / 100.0) << std::endl;

	_tr_error->trace_message(_str_str.str());
}

void logic::CLogic::set_state(e_state state)
{
	_current_state = get_state(state);
}

void logic::CLogic::process_messages_from_device()
{
	std::vector<std::shared_ptr<logic_structures::tag_base_data_from_device>> messages = 
		_packets_from_device.get_with_cleanup();

	for (std::shared_ptr<logic_structures::tag_base_data_from_device> message : messages)
	{
		process_device_message(message);
	}
}

void logic::CLogic::process_device_message(std::shared_ptr<logic_structures::tag_base_data_from_device> message)
{
	byte button_number = 0;
	e_service_name service = e_service_name::stop;
	CSettingsWorkState* sws = nullptr;
	logic_structures::tag_data_from_eeprom* dfe = nullptr;

	switch (message->command_id)
	{
		case(device_exchange::e_command_from_device::bill_acceptor) :
			_current_state->refilled_cache(get_device_message_pointer<logic_structures::tag_bill_acceptor>(message)->count);
			break;

		case(device_exchange::e_command_from_device::button_press) :
			button_number = get_device_message_pointer<logic_structures::tag_button_press>(message)->button_number;
			service = _correspond_settings.GetServiceByButtonNumber(button_number);
			if (e_service_name::stop == service)
				_current_state->stop_button_press();
			else
				_current_state->service_button_press(service);
			break;

		case(device_exchange::e_command_from_device::data_from_eeprom) :
			dfe = get_device_message_pointer<logic_structures::tag_data_from_eeprom>(message);
			sws = dynamic_cast<CSettingsWorkState*>(get_state(e_state::settings_work).get());
			sws->data_from_eeprom(dfe->cell_number, dfe->value);
			break;

		case(device_exchange::e_command_from_device::command_confirmation) :
			_current_state->device_confirm();
			break;
	}

}

std::shared_ptr<logic::IState> logic::CLogic::get_state(e_state state)
{
	return _states[state];
}
