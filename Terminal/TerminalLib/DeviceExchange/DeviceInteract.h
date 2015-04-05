#pragma once
#include "stdafx.h"
#include "lock_vector.h"
#include "device_protocol.h"
#include "TraceError.h"
#include "data_from_pc.h"
#include "data_from_device.h"
#include "tools_structures.h"
#include "LogicDataFromDeviceToPacketFromDeviceConverter.h"
#include "LogicDataFromPcToPacketFromPcConverter.h"
#include "PacketFromDeviceToLogicDataFromDeviceConverter.h"
#include "SingleServerSocket.h"
#include "CommonSettings.h"
#include "DevicePacketConvertor.h"
#include "DeviceMessageDescription.h"

namespace device_exchange
{

/*
CDeviceInteract
Взаимодействие с устройством
*/
class CDeviceInteract
{
	tools::logging::CTraceError* _tr_error;

	// (logic) пакеты для устройства от логики
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_pc>>& _packets_to_device;

	// (logic) пакеты для логики от устройства
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_device>>& _packets_to_logic;

	std::thread _this_thread;

	tools::e_work_loop_status _work_loop_status;
	tools::e_init_state _init_state;

	// (device)пакет от устройства для логики -> (logic)пакет для логики от устройства
	// (device)_packets_from_device -> (logic)_packets_to_logic
	logic_structures::CPacketFromDeviceToLogicDataFromDeviceConverter _packet_from_device_to_packet_to_logic;

	// (logic)пакет для устройства от логики -> (device)пакет от логики для устройства
	// (logic)_packets_to_device -> (device)_packets_from_logic
	logic_structures::CLogicDataFromPcToPacketFromPcConverter _packets_to_device_to_packets_from_logic;

	// подключение к эмулятору устройства
	tools::networking::CSingleServerSocket _device_emulator_connection;
	tools::lock_vector<tools::data_wrappers::_tag_data_managed> _device_raw_data;

	// настройки
	const logic_settings::CCommonSettings& _settings_module;

	// парсер пакетов от устройства
	device_exchange::CDevicePacketConvertor<device_exchange::tag_packet_from_device> _device_packet_parser;

	// создатель пакетов для устройства
	device_exchange::CDevicePacketConvertor<device_exchange::tag_packet_from_pc> _device_packet_creator;

	// описатель пакета от устройства
	// device_exchange::CDeviceMessageDescription _device_message_description;

	// функция потока
	void thread_fn();

	// преобразование устройство -> логика
	void device_to_logic();

	// преобразование логика -> устройство
	void logic_to_device();

	tools::e_init_state init();

public:
	CDeviceInteract(const logic_settings::CCommonSettings& settings_module,
					tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_device>>& packets_to_logic,
					tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_pc>>& packets_to_device);
	virtual ~CDeviceInteract();

	bool Start();

	void Stop();
};
}