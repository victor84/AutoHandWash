#pragma once
#include "TraceError.h"
#include "tools_structures.h"
#include "CommonSettings.h"
#include "CorrespondSettings.h"
#include "DeviceInteract.h"
#include "ServerInteract.h"

/*!
 * \file Logic.h
 *
 * \author Victor Mulika
 * \date 21 Март 2015
 *
 * 
 */

namespace logic
{
/*
CLogic
логика работы терминала
*/
class CLogic
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

	// пакеты для устройства
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_pc>> _packets_to_device;

	// пакеты от устройства
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_device>> _packets_from_device;

	// пакеты от сервера
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>> _packets_from_server;

	// пакеты для сервера
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>> _packets_to_server;

	// модуль обмена с устройством
	device_exchange::CDeviceInteract _device_interact;

	// модуль обмена данных с сервером
	server_exchange::CServerInteract _server_interact;

	// функция текущего потока
	void thread_fn();

	// инициализация
	tools::e_init_state init();

	// соединение с сервером
	void on_connected_to_server();

	// отключение от сервера
	void on_disconnected_from_derver();

	// отправка идентификационного пакета
	void send_identification_packet();

	// отправка пакета с настройками
	void send_settings_packet();

public:
	CLogic();
	~CLogic();

	// запуск в работу
	bool Start();

	// остановка
	void Stop();
};
}



