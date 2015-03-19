#include "stdafx.h"
#include "PacketFromPcToLogicDataFromPcConverter.h"

using namespace logic_structures;
using namespace device_exchange;

CPacketFromPcToLogicDataFromPcConverter::CPacketFromPcToLogicDataFromPcConverter()
{
	fill_handlers();
}

CPacketFromPcToLogicDataFromPcConverter::~CPacketFromPcToLogicDataFromPcConverter()
{
}

std::shared_ptr<tag_base_data_from_pc > CPacketFromPcToLogicDataFromPcConverter::handler_result_to_convert_result(tools::data_wrappers::_tag_data_managed& result)
{
	result.free_after_destruct = false;
	return std::shared_ptr<tag_base_data_from_pc>(reinterpret_cast<tag_base_data_from_pc*>(result.p_data));
}

std::shared_ptr<tag_base_data_from_pc > CPacketFromPcToLogicDataFromPcConverter::get_default_result()
{
	return std::shared_ptr<tag_base_data_from_pc>(new tag_pc_empty());
}

void logic_structures::CPacketFromPcToLogicDataFromPcConverter::fill_handlers()
{
	add_command_from_device_handler(e_command_from_pc::open_valve, std::bind(std::mem_fn(&CPacketFromPcToLogicDataFromPcConverter::convert_open_valve), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_pc::close_valve, std::bind(std::mem_fn(&CPacketFromPcToLogicDataFromPcConverter::convert_close_valve), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_pc::issue_coins, std::bind(std::mem_fn(&CPacketFromPcToLogicDataFromPcConverter::convert_issue_coins), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_pc::issue_discount_card, std::bind(std::mem_fn(&CPacketFromPcToLogicDataFromPcConverter::convert_issue_discount_card), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_pc::read_eeprom, std::bind(std::mem_fn(&CPacketFromPcToLogicDataFromPcConverter::convert_read_eeprom), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_pc::write_eeprom, std::bind(std::mem_fn(&CPacketFromPcToLogicDataFromPcConverter::convert_write_eeprom), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_pc::buttons_status, std::bind(std::mem_fn(&CPacketFromPcToLogicDataFromPcConverter::convert_buttons_status), this, std::placeholders::_1));
}

tools::data_wrappers::_tag_data_managed CPacketFromPcToLogicDataFromPcConverter::convert_open_valve(const tag_packet_from_pc& packet)
{
	tag_open_valve result;

	result.number = packet.data0;

	return result;
}

tools::data_wrappers::_tag_data_managed CPacketFromPcToLogicDataFromPcConverter::convert_close_valve(const tag_packet_from_pc& packet)
{
	tag_close_valve result;

	result.number = packet.data0;

	return result;
}

tools::data_wrappers::_tag_data_managed CPacketFromPcToLogicDataFromPcConverter::convert_issue_coins(const tag_packet_from_pc& packet)
{
	tag_issue_coins result;

	result.number = packet.data0;

	return result;
}

tools::data_wrappers::_tag_data_managed CPacketFromPcToLogicDataFromPcConverter::convert_issue_discount_card(const tag_packet_from_pc& packet)
{
	UNREFERENCED_PARAMETER(packet);

	return tag_issue_discount_card();
}

tools::data_wrappers::_tag_data_managed CPacketFromPcToLogicDataFromPcConverter::convert_read_eeprom(const tag_packet_from_pc& packet)
{
	tag_read_eeprom result;

	result.cell_number = packet.data0;

	return result;
}

tools::data_wrappers::_tag_data_managed CPacketFromPcToLogicDataFromPcConverter::convert_write_eeprom(const tag_packet_from_pc& packet)
{
	tag_write_eeprom result;

	result.cell_number	= packet.data0;
	result.value		= *(uint32_t*)packet.data1;

	return result;
}

tools::data_wrappers::_tag_data_managed CPacketFromPcToLogicDataFromPcConverter::convert_buttons_status(const tag_packet_from_pc& packet)
{
	UNREFERENCED_PARAMETER(packet);
	return tag_buttons_status();
}
