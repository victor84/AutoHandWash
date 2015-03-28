#pragma once
#include "TraceError.h"
#include "tools_structures.h"
#include "CommonSettings.h"
#include "CorrespondSettings.h"
#include "DeviceInteract.h"
#include "ServerInteract.h"
#include "state.h"

/*!
 * \file Logic.h
 *
 * \author Victor Mulika
 * \date 21 ћарт 2015
 *
 * 
 */

namespace logic
{
/*
CLogic
логика работы терминала
*/
class CLogic : CLogicAbstract
{
	tools::logging::CTraceError* _tr_error;

	tools::e_work_loop_status _work_loop_status;

	tools::e_init_state _init_state;

	// объект потока
	std::thread _this_thread;

	// модуль общих натроек
	logic_settings::CCommonSettings _common_settings;

	// модуль соответствий услуг, кнопок и клапанов
	logic_settings::CCorrespondSettings _correspond_settings;

	// пакеты дл€ устройства
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_pc>> _packets_to_device;

	// пакеты от устройства
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_device>> _packets_from_device;

	// пакеты от сервера
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>> _packets_from_server;

	// пакеты дл€ сервера
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>> _packets_to_server;

	// модуль обмена с устройством
	device_exchange::CDeviceInteract _device_interact;

	// модуль обмена данных с сервером
	server_exchange::CServerInteract _server_interact;

	// состо€ни€
	std::map<e_state, std::shared_ptr<IState>> _states;

	// текущее состо€ние
	std::shared_ptr<IState> _current_state;

	// заполнить состо€ни€
	void fill_states();

	// функци€ текущего потока
	void thread_fn();

	// инициализаци€
	tools::e_init_state init();

	// соединение с сервером
	void on_connected_to_server();

	// отключение от сервера
	void on_disconnected_from_derver();

	// отправка идентификационного пакета
	void send_identification_packet();

	// отправка пакета с настройками
	void send_settings_packet();

	virtual std::shared_ptr<IState> get_state(e_state state) final;

	virtual void set_state(e_state state) final;

	// чтение и обработка сообщений от устройства
	void process_messages_from_device();

	// парсинг сообщени€ от устройства
	void process_device_message(std::shared_ptr<logic_structures::tag_base_data_from_device> message);

	template<typename _MessageType>
	_MessageType* get_device_message_pointer(std::shared_ptr<logic_structures::tag_base_data_from_device> message)
	{
		return dynamic_cast<_MessageType*>(message.get());
	}

public:
	CLogic();
	~CLogic();

	// запуск в работу
	bool Start();

	// остановка
	void Stop();

	virtual void open_valve(byte number) final;

	virtual void read_eeprom(byte cell_number) final;

	virtual void write_eeprom(byte cell_number, uint32_t value) final;

};
}



