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
	typedef std::function<bool(const device_exchange::tag_packet_from_device& message)>	_Descriptor_Fn_Type;
	typedef std::pair<device_exchange::e_command_from_device, _Descriptor_Fn_Type>		_Storage_Elem_Type;


	tools::logging::CTraceError* _tr_error;

	std::wstringstream _str_str;

	// хранилище для быстрого доступа к методам-описателям сообщения
	std::map<device_exchange::e_command_from_device, _Descriptor_Fn_Type> _descriptors_storage;

	void fill_descriptors_storage();

	void add_descriptor(device_exchange::e_command_from_device command, bool (CDeviceMessageDescription::*fn)(const device_exchange::tag_packet_from_device&));

	// описать сырые данные
	bool describe_raw(const device_exchange::tag_packet_from_device& message);

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

public:
	CDeviceMessageDescription();
	virtual ~CDeviceMessageDescription();

	// Описать информацию о сообщении
	std::wstring describe_message(const device_exchange::tag_packet_from_device& message);
};
}