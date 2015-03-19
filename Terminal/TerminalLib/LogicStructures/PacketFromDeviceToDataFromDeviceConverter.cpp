#include "stdafx.h"
#include "PacketFromDeviceToDataFromDeviceConverter.h"
#include "device_protocol.h"

using namespace logic_structures;
using namespace device_exchange;

CPacketFromDeviceToDataFromDeviceConverter::CPacketFromDeviceToDataFromDeviceConverter()
{
	fill_handlers();
}

CPacketFromDeviceToDataFromDeviceConverter::~CPacketFromDeviceToDataFromDeviceConverter()
{
}

std::shared_ptr<tag_base_data_from_device > CPacketFromDeviceToDataFromDeviceConverter::handler_result_to_convert_result(tools::data_wrappers::_tag_data_managed& result)
{
	result.free_after_destruct = false;
	return std::shared_ptr<tag_base_data_from_device>(reinterpret_cast<tag_base_data_from_device*>(result.p_data));
}

std::shared_ptr<tag_base_data_from_device > CPacketFromDeviceToDataFromDeviceConverter::get_default_result()
{
	return std::shared_ptr<tag_base_data_from_device>(new tag_dev_empty());
}

void CPacketFromDeviceToDataFromDeviceConverter::fill_handlers()
{
	add_command_from_device_handler(e_command_from_device::button_press, std::bind(std::mem_fn(&CPacketFromDeviceToDataFromDeviceConverter::convert_button_press), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_device::bill_acceptor, std::bind(std::mem_fn(&CPacketFromDeviceToDataFromDeviceConverter::convert_bill_acceptor), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_device::hopper_issue_coin, std::bind(std::mem_fn(&CPacketFromDeviceToDataFromDeviceConverter::convert_hopper_issue_coin), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_device::discount_card_issued, std::bind(std::mem_fn(&CPacketFromDeviceToDataFromDeviceConverter::convert_discount_card_issued), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_device::data_from_eeprom, std::bind(std::mem_fn(&CPacketFromDeviceToDataFromDeviceConverter::convert_data_from_eeprom), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_device::buttons_state, std::bind(std::mem_fn(&CPacketFromDeviceToDataFromDeviceConverter::convert_buttons_state), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_device::command_confirmation, std::bind(std::mem_fn(&CPacketFromDeviceToDataFromDeviceConverter::convert_command_confirmation), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_device::error, std::bind(std::mem_fn(&CPacketFromDeviceToDataFromDeviceConverter::convert_error), this, std::placeholders::_1));
}

tools::data_wrappers::_tag_data_managed CPacketFromDeviceToDataFromDeviceConverter::convert_button_press(const device_exchange::tag_packet_from_device& packet_from_device)
{
	tag_button_press result;

	result.button_number = packet_from_device.data0;

	return result;
}

tools::data_wrappers::_tag_data_managed CPacketFromDeviceToDataFromDeviceConverter::convert_bill_acceptor(const device_exchange::tag_packet_from_device& packet_from_device)
{
	tag_bill_acceptor result;

	result.count = (*(uint16_t*)&packet_from_device.data0);

	return result;
}

tools::data_wrappers::_tag_data_managed CPacketFromDeviceToDataFromDeviceConverter::convert_hopper_issue_coin(const device_exchange::tag_packet_from_device& packet_from_device)
{
	tag_hopper_issue_coin result;

	result.balance = packet_from_device.data0;

	return result;
}

tools::data_wrappers::_tag_data_managed CPacketFromDeviceToDataFromDeviceConverter::convert_discount_card_issued(const device_exchange::tag_packet_from_device& packet_from_device)
{
	UNREFERENCED_PARAMETER(packet_from_device);
	return tag_discount_card_issued();
}

tools::data_wrappers::_tag_data_managed CPacketFromDeviceToDataFromDeviceConverter::convert_data_from_eeprom(const device_exchange::tag_packet_from_device& packet_from_device)
{
	tag_data_from_eeprom result;

	result.cell_number = packet_from_device.data0;
	result.value = (*(uint32_t*)&packet_from_device.data1);

	return result;
}

tools::data_wrappers::_tag_data_managed CPacketFromDeviceToDataFromDeviceConverter::convert_buttons_state(const device_exchange::tag_packet_from_device& packet_from_device)
{
	tag_buttons_state result;

	uint16_t state = (*(uint16_t*)&packet_from_device.data0);

	result.button1 = (0 != (state & 0x01));
	result.button2 = (0 != ((state >> 1) & 0x01));
	result.button3 = (0 != ((state >> 2) & 0x01));
	result.button4 = (0 != ((state >> 3) & 0x01));
	result.button5 = (0 != ((state >> 4) & 0x01));
	result.button6 = (0 != ((state >> 5) & 0x01));
	result.button7 = (0 != ((state >> 6) & 0x01));
	result.button8 = (0 != ((state >> 7) & 0x01));
	result.button9 = (0 != ((state >> 8) & 0x01));

	return result;
}

tools::data_wrappers::_tag_data_managed CPacketFromDeviceToDataFromDeviceConverter::convert_command_confirmation(const device_exchange::tag_packet_from_device& packet_from_device)
{
	tag_command_confirmation result;

	result.command = static_cast<e_command_from_pc>(packet_from_device.data0);

	return result;
}

tools::data_wrappers::_tag_data_managed CPacketFromDeviceToDataFromDeviceConverter::convert_error(const device_exchange::tag_packet_from_device& packet_from_device)
{
	tag_error result;

	result.code = static_cast<e_device_error_code>(packet_from_device.data0);

	return result;
}


