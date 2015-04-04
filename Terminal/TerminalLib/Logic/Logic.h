#pragma once
#include "TraceError.h"
#include "tools_structures.h"
#include "CommonSettings.h"
#include "CorrespondSettings.h"
#include "DeviceInteract.h"
#include "ServerInteract.h"
#include "state.h"
#include "ILogic.h"

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
class CLogic : CLogicAbstract, public ILogic
{
	tools::logging::CTraceError* _tr_error;

	std::wstringstream _str_str;

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

	// состояния
	std::map<e_state, std::shared_ptr<IState>> _states;

	// текущее состояние
	std::shared_ptr<IState> _current_state;

	// вызывается при изменении денег и времени
	std::function<void(int16_t, int16_t)> _on_change_time_and_money_fn;

	// вызывается при изменении состояния
	std::function<void(e_service_name, std::wstring)> _on_service_changed_fn;

	// вызывается при изменении состояния
	std::function<void(e_state)> _on_state_changed_fn;

	// вызывается при пополнении счёта
	std::function<void(uint16_t)> _on_cache_refilled;

	// вызывается после прочтения настроек услуг
	std::function<void(std::vector<tag_service_info>)> _on_service_info_readed;

	// заполнить состояния
	void fill_states();

	// функция текущего потока
	void thread_fn();

	// инициализация
	tools::e_init_state init();

	// отправка получателю информации об услугах
	void send_services_info();

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

	// парсинг сообщения от устройства
	void process_device_message(std::shared_ptr<logic_structures::tag_base_data_from_device> message);

	template<typename _MessageType>
	_MessageType* get_device_message_pointer(std::shared_ptr<logic_structures::tag_base_data_from_device> message)
	{
		return dynamic_cast<_MessageType*>(message.get());
	}

	virtual void open_valve(byte number) final;

	virtual void read_eeprom(byte cell_number) final;

	virtual void write_eeprom(byte cell_number, uint32_t value) final;

	virtual void time_and_money(int16_t time, int16_t money) final;

	virtual void close_valve(byte number) final;

	// запуск в работу
	virtual bool Start() final;

	// остановка
	virtual void Stop() final;

	virtual void SetOnTimeAndMoneyFn(std::function<void(int16_t, int16_t) > fn) final;

	virtual void SetOnServiceChangedFn(std::function<void(e_service_name, std::wstring) > fn) final;

	virtual void SetOnStateChangedFn(std::function<void(e_state)> fn) final;

	virtual void SetOnCacheRefilledFn(std::function<void(uint16_t) > fn) final;

	virtual void SetOnServiceInfoReadedFn(std::function<void(std::vector<tag_service_info>) > fn) final;

public:
	CLogic();
	~CLogic();



};
}



