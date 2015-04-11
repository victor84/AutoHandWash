#pragma once
#include "stdafx.h"
#include "TraceError.h"
#include "AsyncClientSocket.h"
#include "ServerPacketParser.h"
#include "PacketToRawData.h"
#include "CommonSettings.h"
#include "server_structures.h"

namespace server_exchange
{
/*
CServerInteract
ќбмен данными с сервером
*/
class CServerInteract
{
	tools::logging::CTraceError* _tr_error;

	// сырые данные с сервера
	tools::lock_vector<tools::data_wrappers::_tag_data_managed> _raw_data_from_server;

	// соединение с сервером
	tools::networking::CAsyncClientSocket _client_socket;

	// парсер пакетов от сервера
	server_exchange::CServerPacketParser _parser;

	// преобразователь пакетов в сырые данные
	server_exchange::CPacketToRawData _packet_to_raw_data;

	// пакеты дл€ логики
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>>& _packets_to_logic;

	// пакеты дл€ сервера
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>>& _packets_to_server;

	// сост€ние инициализации
	tools::e_init_state _init_state;

	// состо€ние рабочего потока
	tools::e_work_loop_status _work_loop_status;

	// рабочий поток
	std::thread _this_thread;

	// настройки
	logic_settings::CCommonSettings& _settings_module;

	// метод потока
	void thread_fn();

	bool parse_from_server();

	bool parse_transport_packet(const tag_transport_packet& transport_packet);

	bool send_to_server();

	bool send_packet_to_server(std::shared_ptr<logic_structures::tag_base_server_logic_struct> packet, bool to_front = false);

	template<typename _StructType, server_exchange::e_packet_type id>
	_StructType get_server_logic_packet(const std::shared_ptr<logic_structures::tag_base_server_logic_struct>& base_packet)
	{
		const logic_structures::tag_server_logic_packet<_StructType, id>* p_lp = 
			dynamic_cast<const logic_structures::tag_server_logic_packet<_StructType, id>*>(base_packet.get());

		return p_lp->packet;
	}

public:
	CServerInteract(logic_settings::CCommonSettings& settings_module,
					tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>>& packets_to_logic,
					tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>>& packets_to_server,
					std::function<void(void)> on_connected,
					std::function<void(void)> on_disconnected);
	virtual ~CServerInteract();

	bool Start();

	void Stop();

	bool IsStarted();

	// ќтправить пакет перед остальными
	void PushFrontToSend(std::shared_ptr<logic_structures::tag_base_server_logic_struct> packet);

	// ѕоставить в очередь на отправку
	void PushBackToSend(std::shared_ptr<logic_structures::tag_base_server_logic_struct> packet);
};
}



