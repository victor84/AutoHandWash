#pragma once
#include "TraceError.h"
#include "device_protocol.h"
/*!
 * \file DeveiceMessageShower.h
 *
 * \author Victor Mulika
 * \date 09 Март 2015
 *
 * 
 */

namespace device_exchange
{
/*
CDeveiceMessageShower
отображает информацию о сообщении
*/
class CDeviceMessageDescription
{
	typedef std::function<bool(const device_exchange::tag_packet_from_device& message)>		_Device_Descriptor_Fn_Type;
	typedef std::pair<device_exchange::e_command_from_device, _Device_Descriptor_Fn_Type>	_Device_Storage_Elem_Type;

	typedef std::function<bool(const device_exchange::tag_packet_from_pc& message)>		_Pc_Descriptor_Fn_Type;
	typedef std::pair<device_exchange::e_command_from_pc, _Pc_Descriptor_Fn_Type>	_Pc_Storage_Elem_Type;

	tools::logging::CTraceError* _tr_error;

	std::wstringstream _str_str;

	// хранилище для быстрого доступа к методам-описателям сообщения
	std::map<device_exchange::e_command_from_device, _Device_Descriptor_Fn_Type> _device_descriptors_storage;
	std::map<device_exchange::e_command_from_pc, _Pc_Descriptor_Fn_Type> _pc_descriptors_storage;

	void fill_descriptors_storage();

	void add_descriptor(device_exchange::e_command_from_device command, bool (CDeviceMessageDescription::*fn)(const device_exchange::tag_packet_from_device&));
	void add_descriptor(device_exchange::e_command_from_pc command, bool (CDeviceMessageDescription::*fn)(const device_exchange::tag_packet_from_pc&));


	// описать сырые данные
	bool device_describe_raw(const device_exchange::tag_packet_from_device& message);

	// описатель e_command_from_device::button_press
	bool button_press(const device_exchange::tag_packet_from_device& message);

	// описатель e_command_from_device::bill_acceptor
	bool bill_acceptor(const device_exchange::tag_packet_from_device& message);

	// описатель e_command_from_device::hopper_issue_coin
	bool hopper_issue_coin(const device_exchange::tag_packet_from_device& message);

	// описатель e_command_from_device::discount_card_issued
	bool discount_card_issued(const device_exchange::tag_packet_from_device& message);

	// описатель e_command_from_device::data_from_eeprom
	bool data_from_eeprom(const device_exchange::tag_packet_from_device& message);

	// описатель e_command_from_device::buttons_state
	bool buttons_state(const device_exchange::tag_packet_from_device& message);

	// описатель e_command_from_device::command_confirmation
	bool command_confirmation(const device_exchange::tag_packet_from_device& message);

	// описатель e_command_from_device::error
	bool error(const device_exchange::tag_packet_from_device& message);



	// описать сырые данные
	bool pc_describe_raw(const device_exchange::tag_packet_from_pc& message);

	// описатель e_command_from_pc::open_valve
	bool open_valve(const device_exchange::tag_packet_from_pc& message);

	// описатель e_command_from_pc::close_valve
	bool close_valve(const device_exchange::tag_packet_from_pc& message);

	// описатель e_command_from_pc::issue_coins
	bool issue_coins(const device_exchange::tag_packet_from_pc& message);

	// описатель e_command_from_pc::issue_discount_card
	bool issue_discount_card(const device_exchange::tag_packet_from_pc& message);

	// описатель e_command_from_pc::read_eeprom
	bool read_eeprom(const device_exchange::tag_packet_from_pc& message);

	// описатель e_command_from_pc::write_eeprom
	bool write_eeprom(const device_exchange::tag_packet_from_pc& message);

	// описатель e_command_from_pc::buttons_status
	bool buttons_status(const device_exchange::tag_packet_from_pc& message);

public:
	CDeviceMessageDescription();
	virtual ~CDeviceMessageDescription();

	// Описать информацию о сообщении от устройства
	std::wstring describe_message(const device_exchange::tag_packet_from_device& message);

	// Описать информацию о сообщении от pc
	std::wstring describe_message(const device_exchange::tag_packet_from_pc& message);
};
}