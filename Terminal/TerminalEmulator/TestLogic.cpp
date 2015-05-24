#include "stdafx.h"
#include "TestLogic.h"

using namespace server_exchange;
using namespace device_exchange;
using namespace logic_structures;

void CTestLogic::thread_method()
{
	if (false == send_identification_packet())
		_continue_work = false;

	send_settings_packet();

	while (true == _continue_work)
	{
		// send_counters_packet();

		// send_log_record_packet();

		// std::this_thread::sleep_for(std::chrono::seconds(20));
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void CTestLogic::on_server_data_received(tools::data_wrappers::_tag_data_managed data)
{

}

void CTestLogic::on_device_data_received(tools::data_wrappers::_tag_data_managed data)
{
	std::vector<tag_packet_from_device> packets;

	_device_packet_from_device_convertor.Parse(data, packets);

	for (tag_packet_from_device packet : packets)
	{
		_tr_error->trace_message(_device_message_descriptor.describe_message(packet));
		_received_device_packet = _device_structures_converter.Convert(packet, static_cast<e_command_from_device>(packet.command));
		process_device_packet();
	}
}

void CTestLogic::process_device_packet()
{
	tag_bill_acceptor* ba		  = nullptr;
	tag_button_press* bp		  = nullptr;
	tag_buttons_state* bs		  = nullptr;
	tag_command_confirmation* cc  = nullptr;
	tag_data_from_eeprom* dfe	  = nullptr;
	tag_discount_card_issued* dci = nullptr;
	tag_dev_empty* de			  = nullptr;
	tag_error* e				  = nullptr;
	tag_hopper_issue_coin* hic	  = nullptr;

	switch (_received_device_packet->command_id)
	{
		case e_command_from_device::bill_acceptor : 
			ba = static_cast<tag_bill_acceptor*>(_received_device_packet.get());
			break;
		case e_command_from_device::button_press :
			bp = static_cast<tag_button_press*>(_received_device_packet.get());
			break;
		case e_command_from_device::buttons_state :
			bs = static_cast<tag_buttons_state*>(_received_device_packet.get());
			break;
		case e_command_from_device::command_confirmation :
			cc = static_cast<tag_command_confirmation*>(_received_device_packet.get());
			break;
		case e_command_from_device::data_from_eeprom :
			dfe = static_cast<tag_data_from_eeprom*>(_received_device_packet.get());
			break;
		case e_command_from_device::discount_card_issued :
			dci =static_cast<tag_discount_card_issued*>(_received_device_packet.get());
			break;
		case e_command_from_device::empty :
			de = static_cast<tag_dev_empty*>(_received_device_packet.get());
			break;
		case e_command_from_device::error :
			e = static_cast<tag_error*>(_received_device_packet.get());
			break;
		case e_command_from_device::hopper_issue_coin :
			hic = static_cast<tag_hopper_issue_coin*>(_received_device_packet.get());
			break;
	}
}

bool CTestLogic::send_identification_packet()
{
	tag_identification_packet identification_packet;

	std::wstring group_name = _T("Хорошая группа");
	group_name._Copy_s(identification_packet.group_name, 
					   sizeof(identification_packet.group_name), 
					   group_name.size());

	std::wstring terminal_name = _T("Первый терминал");
	terminal_name._Copy_s(identification_packet.terminal_name, 
						  sizeof(identification_packet.terminal_name), 
						  terminal_name.size());

	tag_transport_packet transport_packet;
	transport_packet.length = sizeof(identification_packet);
	transport_packet.type = e_packet_type::id;
	_packet_to_raw_data.CreateIdentificationPacketRawData(identification_packet, transport_packet.data);

	tools::data_wrappers::_tag_data_managed raw_data;

	_packet_to_raw_data.CreateRawData(transport_packet, raw_data);

	_client_socket.PushBackToSend(raw_data);

	return true;
}

bool CTestLogic::send_settings_packet()
{
	tag_settings_packet settings_packet;

	settings_packet.against_midges = 10;
	settings_packet.air = 5;
	settings_packet.bill_acceptor_impulse = 10;
	settings_packet.coin_acceptor_impulse = 11;
	settings_packet.foam = 30;
	settings_packet.free_idle_time = 2;
	settings_packet.idle_time_cost = 10;
	settings_packet.osmosis = 60;
	settings_packet.pause_before_advertising = 1;
	settings_packet.pressurized_water = 5;
	settings_packet.state = logic::e_terminal_state::work;
	settings_packet.vacuum_cleaner = 35;
	settings_packet.water_without_pressure = 3;
	settings_packet.wax = 50;

	tag_transport_packet transport_packet;

	transport_packet.type = e_packet_type::settings;
	transport_packet.length = sizeof(settings_packet);
	_packet_to_raw_data.CreateSettingsPacketRawData(settings_packet, transport_packet.data);

	tools::data_wrappers::_tag_data_managed raw_data;

	_packet_to_raw_data.CreateRawData(transport_packet, raw_data);

	_client_socket.PushBackToSend(raw_data);

	return true;
}

bool CTestLogic::send_counters_packet()
{
	tag_counters_packet counters_packet;

	counters_packet.against_midges = 10;
	counters_packet.air = 30;
	counters_packet.current_cache = 50;
	counters_packet.date_time = std::time(0);
	counters_packet.foam = 11;
	counters_packet.osmosis = 7;
	counters_packet.pressurized_water = 50;
	counters_packet.state = logic::e_terminal_state::work;
	counters_packet.total_cache = 2000;
	counters_packet.vacuum_cleaner = 12;
	counters_packet.water_without_pressure = 55;
	counters_packet.wax = 8;

	tag_transport_packet transport_packet;

	transport_packet.type = e_packet_type::counters;
	transport_packet.length = sizeof(counters_packet);
	_packet_to_raw_data.CreateCountersPacketRawData(counters_packet, transport_packet.data);

	tools::data_wrappers::_tag_data_managed raw_data;

	_packet_to_raw_data.CreateRawData(transport_packet, raw_data);

	_client_socket.PushBackToSend(raw_data);

	return true;
}

bool CTestLogic::send_log_record_packet()
{
	tag_log_record_packet log_record_packet;

	log_record_packet.date_time = std::time(0);
	log_record_packet.type = e_log_record_type::message;

	std::wstring message = _T("Пиздатое сообщение в лог");
	uint16_t message_size = static_cast<uint16_t>(message.size() * 2);

	log_record_packet.text.copy_data_inside(message.c_str(), message_size);
	log_record_packet.length = message_size;

	uint32_t packet_size = 11 + log_record_packet.length;

	tag_transport_packet transport_packet;

	transport_packet.type = e_packet_type::log;
	transport_packet.length = packet_size;
	_packet_to_raw_data.CreateLogRecordPacketRawData(log_record_packet, transport_packet.data);

	tools::data_wrappers::_tag_data_managed raw_data;

	_packet_to_raw_data.CreateRawData(transport_packet, raw_data);

	_client_socket.PushBackToSend(raw_data);

	return true;

}

CTestLogic::CTestLogic()
	: _client_socket(_received_server_data, std::bind(std::mem_fn(&CTestLogic::on_server_data_received), this, std::placeholders::_1), nullptr, nullptr)
	, _device_socket(_received_device_data, std::bind(std::mem_fn(&CTestLogic::on_device_data_received), this, std::placeholders::_1))
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

CTestLogic::~CTestLogic()
{
}

bool CTestLogic::Start(tools::networking::tag_connection_params server_connection_params, tools::networking::tag_connection_params device_connection_params)
{
	if (_continue_work == true)
		return true;

	_client_socket.OpenConnection(server_connection_params);

	if (tools::networking::e_socket_result::success == _device_socket.Start(device_connection_params))
	{
		_tr_error->trace_message(_T("\r\nСервер запущен\r\n"));
	}
	else
	{
		_tr_error->trace_message(_T("\r\nОшибка запуска сервера\r\n"));
	}

	_continue_work = true;
	_this_thread = std::thread(&CTestLogic::thread_method, this);
	
	return true;
}

void CTestLogic::Stop()
{
	if (false == _continue_work)
		return;

	_continue_work = false;

	if (true == _this_thread.joinable())
		_this_thread.join();

	_client_socket.CloseConnection();
	_device_socket.Stop();
}
