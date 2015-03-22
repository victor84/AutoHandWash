#include "stdafx.h"
#include "ServerInteract.h"
#include "server_structures.h"
#include "tools.h"

void server_exchange::CServerInteract::thread_fn()
{
	while (tools::e_work_loop_status::ok == _work_loop_status)
	{
		parse_from_server();
		send_to_server();

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

bool server_exchange::CServerInteract::parse_from_server()
{
	std::vector<tools::data_wrappers::_tag_data_const> raw_data = _raw_data_from_server.get_with_cleanup();

	for (tools::data_wrappers::_tag_data_const data : raw_data)
	{
		std::vector<tag_transport_packet> transport_packets;

		if (e_convert_result::success != _parser.ParseTransportPacket(data, transport_packets))
		{
			_tr_error->trace_error(_T("Не удалось распарсить данные от сервера"));
			_tr_error->trace_error(tools::binary_to_hex(data));
			continue;
		}

		for (tag_transport_packet transport_packet : transport_packets)
		{
			parse_transport_packet(transport_packet);
		}
	}
	return true;
}

bool server_exchange::CServerInteract::parse_transport_packet(const tag_transport_packet transport_packet)
{
	tag_settings_packet settings_packet;
	tag_confirmation_packet confirmation_packet;
	logic_structures::tag_base_server_logic_struct* pointer = nullptr;
	std::shared_ptr<logic_structures::tag_base_server_logic_struct> shared_pointer;

	switch (transport_packet.type)
	{
		case(e_packet_type::settings) :
			if (e_convert_result::success == _parser.ParseSettingsPacket(transport_packet, settings_packet))
			{
				logic_structures::tag_server_logic_packet<tag_settings_packet, e_packet_type::settings> sp(settings_packet);
				pointer = &sp;
			}
			break;
		case (e_packet_type::confirmation) :
			if (e_convert_result::success == _parser.ParseConfirmationPacket(transport_packet, confirmation_packet))
			{
				logic_structures::tag_server_logic_packet<tag_confirmation_packet, e_packet_type::confirmation> cp(confirmation_packet);
				pointer = &cp;
			}
			break;
		default:
			_tr_error->trace_error(_T("Не удалось распарсить сообщение от сервера"));
			_tr_error->trace_error(tools::binary_to_hex(
				tools::data_wrappers::_tag_data_const(reinterpret_cast<const byte*>(&transport_packet),
				sizeof(transport_packet))));
			return false;
	}

	if (nullptr != pointer)
	{
		shared_pointer = std::shared_ptr<logic_structures::tag_base_server_logic_struct>(pointer);
		_packets_to_logic.push_back(shared_pointer);
		return true;
	}

	return false;
}

bool server_exchange::CServerInteract::send_to_server()
{
	std::vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>> packets = _packets_to_server.get_with_cleanup();

	for (std::shared_ptr<logic_structures::tag_base_server_logic_struct> packet : packets)
	{
		send_packet_to_server(packet);
	}
	return true;
}

bool server_exchange::CServerInteract::send_packet_to_server(std::shared_ptr<logic_structures::tag_base_server_logic_struct> packet, bool to_front)
{
	tools::data_wrappers::_tag_data_managed raw_data;
	switch (packet->type)
	{
		case (e_packet_type::confirmation) :
			_packet_to_raw_data.CreateConfirmationPacketRawData(*(reinterpret_cast<tag_confirmation_packet*>(packet.get())), raw_data);
			break;
		case (e_packet_type::counters) :
			_packet_to_raw_data.CreateCountersPacketRawData(*(reinterpret_cast<tag_counters_packet*>(packet.get())), raw_data);
			break;
		case (e_packet_type::id) :
			_packet_to_raw_data.CreateIdentificationPacketRawData(*(reinterpret_cast<tag_identification_packet*>(packet.get())), raw_data);
			break;
		case (e_packet_type::log) :
			_packet_to_raw_data.CreateLogRecordPacketRawData(*(reinterpret_cast<tag_log_record_packet*>(packet.get())), raw_data);
			break;
		case (e_packet_type::settings) :
			_packet_to_raw_data.CreateSettingsPacketRawData(*(reinterpret_cast<tag_settings_packet*>(packet.get())), raw_data);
			break;
		default:
			_tr_error->trace_error(_T("Попытка отправить на сервер пакет неизвестного типа"));
			break;
	}

	if (nullptr != raw_data.p_data)
	{
		if (true == to_front)
			_client_socket.PushFrontToSend(raw_data);
		else
			_client_socket.PushBackToSend(raw_data);

		return true;
	}
	return false;
}

server_exchange::CServerInteract::CServerInteract(logic_settings::CCommonSettings& settings_module,
												  tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>>& packets_to_logic,
												  tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>>& packets_to_server,
												  std::function<void(void)> on_connected,
												  std::function<void(void)> on_disconnected)
	: _client_socket(_raw_data_from_server, nullptr, on_connected, on_disconnected)
	, _settings_module(settings_module)
	, _packets_to_logic(packets_to_logic)
	, _packets_to_server(packets_to_server)
{
}

server_exchange::CServerInteract::~CServerInteract()
{
}

bool server_exchange::CServerInteract::Start()
{
	if (tools::e_work_loop_status::ok == _work_loop_status)
		return true;

	tools::networking::tag_connection_params connection_params;

	connection_params.address = _settings_module.GetServerAddress();
	connection_params.port = _settings_module.GetServerPort();

	if (tools::networking::e_socket_result::success != _client_socket.OpenConnection(connection_params))
	{
		_tr_error->trace_error(_T("Не удалось с первой попытки подключиться к серверу."));
	}

	_this_thread = std::thread(&CServerInteract::thread_fn, this);

	_init_state = tools::e_init_state::was_init;
	return true;
}

void server_exchange::CServerInteract::Stop()
{
	_work_loop_status = tools::e_work_loop_status::stop;

	if (true == _this_thread.joinable())
		_this_thread.join();

	_client_socket.CloseConnection();

	_init_state = tools::e_init_state::not_init;
}

void server_exchange::CServerInteract::PushFrontToSend(std::shared_ptr<logic_structures::tag_base_server_logic_struct> packet)
{
	send_packet_to_server(packet, true);
}
