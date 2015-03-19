#pragma once
#include "stdafx.h"
#include "BaseStructuresConvertor.h"
#include "raw_data_warappers.h"
#include "device_protocol.h"
#include "data_from_device.h"

namespace logic_structures
{

class CPacketFromDeviceToDataFromDeviceConverter : public CBaseStructuresConvertor<
															typename tools::data_wrappers::_tag_data_managed,
															typename device_exchange::tag_packet_from_device,
															typename device_exchange::e_command_from_device,
															typename std::shared_ptr<tag_base_data_from_device >,
															CPacketFromDeviceToDataFromDeviceConverter>
{
	virtual std::shared_ptr<tag_base_data_from_device > handler_result_to_convert_result(tools::data_wrappers::_tag_data_managed& result) final;

	virtual std::shared_ptr<tag_base_data_from_device > get_default_result() final;

	virtual void fill_handlers() final;

	// преобразователи устройство -> логика
	tools::data_wrappers::_tag_data_managed convert_button_press(const device_exchange::tag_packet_from_device& packet_from_device);
	tools::data_wrappers::_tag_data_managed convert_bill_acceptor(const device_exchange::tag_packet_from_device& packet_from_device);
	tools::data_wrappers::_tag_data_managed convert_hopper_issue_coin(const device_exchange::tag_packet_from_device& packet_from_device);
	tools::data_wrappers::_tag_data_managed convert_discount_card_issued(const device_exchange::tag_packet_from_device& packet_from_device);
	tools::data_wrappers::_tag_data_managed convert_data_from_eeprom(const device_exchange::tag_packet_from_device& packet_from_device);
	tools::data_wrappers::_tag_data_managed convert_buttons_state(const device_exchange::tag_packet_from_device& packet_from_device);
	tools::data_wrappers::_tag_data_managed convert_command_confirmation(const device_exchange::tag_packet_from_device& packet_from_device);
	tools::data_wrappers::_tag_data_managed convert_error(const device_exchange::tag_packet_from_device& packet_from_device);

public:
	CPacketFromDeviceToDataFromDeviceConverter();
	virtual ~CPacketFromDeviceToDataFromDeviceConverter();

};

}