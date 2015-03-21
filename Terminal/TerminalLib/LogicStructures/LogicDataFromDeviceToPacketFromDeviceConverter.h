#pragma once
#include "stdafx.h"
#include "BaseStructuresConvertor.h"
#include "raw_data_warappers.h"
#include "device_protocol.h"
#include "data_from_device.h"

/*!
 * \file LogicPacketFromDeviceToPacketFromDeviceConverter.h
 *
 * \author Victor Mulika
 * \date 19 ћарт 2015
 *
 * 
 */

namespace logic_structures
{
/*
CLogicDataFromDeviceToPacketFromDeviceConverter
logic packet from device -> hardware device packet
*/
class CLogicDataFromDeviceToPacketFromDeviceConverter : public CBaseStructuresConvertor <
														typename device_exchange::tag_packet_from_device,
														typename std::shared_ptr<tag_base_data_from_device>,
														typename device_exchange::e_command_from_device,
														typename device_exchange::tag_packet_from_device,
														CLogicDataFromDeviceToPacketFromDeviceConverter>
{
	virtual void fill_handlers() final;

	virtual device_exchange::tag_packet_from_device handler_result_to_convert_result(device_exchange::tag_packet_from_device& result) final;

	virtual device_exchange::tag_packet_from_device get_default_result() final;

	// преобразователи логика -> устройство
	device_exchange::tag_packet_from_device convert_button_press		(const std::shared_ptr<tag_base_data_from_device>& packet);
	device_exchange::tag_packet_from_device convert_bill_acceptor		(const std::shared_ptr<tag_base_data_from_device>& packet);
	device_exchange::tag_packet_from_device convert_hopper_issue_coin	(const std::shared_ptr<tag_base_data_from_device>& packet);
	device_exchange::tag_packet_from_device convert_discount_card_issued(const std::shared_ptr<tag_base_data_from_device>& packet);
	device_exchange::tag_packet_from_device convert_data_from_eeprom	(const std::shared_ptr<tag_base_data_from_device>& packet);
	device_exchange::tag_packet_from_device convert_buttons_state		(const std::shared_ptr<tag_base_data_from_device>& packet);
	device_exchange::tag_packet_from_device convert_command_confirmation(const std::shared_ptr<tag_base_data_from_device>& packet);
	device_exchange::tag_packet_from_device convert_error				(const std::shared_ptr<tag_base_data_from_device>& packet);

public:
	CLogicDataFromDeviceToPacketFromDeviceConverter();
	virtual ~CLogicDataFromDeviceToPacketFromDeviceConverter();

};

}