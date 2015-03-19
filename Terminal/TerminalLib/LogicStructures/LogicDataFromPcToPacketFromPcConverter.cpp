#include "stdafx.h"
#include "LogicDataFromPcToPacketFromPcConverter.h"

using namespace logic_structures;
using namespace device_exchange;

CLogicDataFromPcToPacketFromPcConverter::CLogicDataFromPcToPacketFromPcConverter()
{
	fill_handlers();
}


CLogicDataFromPcToPacketFromPcConverter::~CLogicDataFromPcToPacketFromPcConverter()
{
}

void CLogicDataFromPcToPacketFromPcConverter::fill_handlers()
{
	add_command_from_device_handler(e_command_from_pc::open_valve, std::bind(std::mem_fn(&CLogicDataFromPcToPacketFromPcConverter::convert_open_valve), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_pc::close_valve, std::bind(std::mem_fn(&CLogicDataFromPcToPacketFromPcConverter::convert_close_valve), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_pc::issue_coins, std::bind(std::mem_fn(&CLogicDataFromPcToPacketFromPcConverter::convert_issue_coins), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_pc::issue_discount_card, std::bind(std::mem_fn(&CLogicDataFromPcToPacketFromPcConverter::convert_issue_discount_card), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_pc::read_eeprom, std::bind(std::mem_fn(&CLogicDataFromPcToPacketFromPcConverter::convert_read_eeprom), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_pc::write_eeprom, std::bind(std::mem_fn(&CLogicDataFromPcToPacketFromPcConverter::convert_write_eeprom), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_pc::buttons_status, std::bind(std::mem_fn(&CLogicDataFromPcToPacketFromPcConverter::convert_buttons_status), this, std::placeholders::_1));
}

tag_packet_from_pc CLogicDataFromPcToPacketFromPcConverter::handler_result_to_convert_result(tag_packet_from_pc& result)
{
	return result;
}

tag_packet_from_pc CLogicDataFromPcToPacketFromPcConverter::get_default_result()
{
	return tag_packet_from_pc();
}

tag_packet_from_pc CLogicDataFromPcToPacketFromPcConverter::convert_open_valve(const std::shared_ptr<tag_base_data_from_pc>& packet)
{
	tag_packet_from_pc result;

	tag_open_valve* p = reinterpret_cast<tag_open_valve*>(packet.get());

	result.command = p->command_id;
	result.data0 = p->number;

	return result;
}

tag_packet_from_pc CLogicDataFromPcToPacketFromPcConverter::convert_close_valve(const std::shared_ptr<tag_base_data_from_pc>& packet)
{
	tag_packet_from_pc result;

	tag_close_valve* p = reinterpret_cast<tag_close_valve*>(packet.get());

	result.command = p->command_id;
	result.data0 = p->number;

	return result;
}

tag_packet_from_pc logic_structures::CLogicDataFromPcToPacketFromPcConverter::convert_issue_coins(const std::shared_ptr<tag_base_data_from_pc>& packet)
{
	tag_packet_from_pc result;

	tag_issue_coins* p = reinterpret_cast<tag_issue_coins*>(packet.get());

	result.command = p->command_id;
	result.data0 = p->number;

	return result;
}

tag_packet_from_pc logic_structures::CLogicDataFromPcToPacketFromPcConverter::convert_issue_discount_card(const std::shared_ptr<tag_base_data_from_pc>& packet)
{
	tag_packet_from_pc result;

	tag_issue_discount_card* p = reinterpret_cast<tag_issue_discount_card*>(packet.get());

	result.command = p->command_id;

	return result;
}

tag_packet_from_pc logic_structures::CLogicDataFromPcToPacketFromPcConverter::convert_read_eeprom(const std::shared_ptr<tag_base_data_from_pc>& packet)
{
	tag_packet_from_pc result;

	tag_read_eeprom* p = reinterpret_cast<tag_read_eeprom*>(packet.get());

	result.command = p->command_id;
	result.data0 = p->cell_number;

	return result;
}

tag_packet_from_pc logic_structures::CLogicDataFromPcToPacketFromPcConverter::convert_write_eeprom(const std::shared_ptr<tag_base_data_from_pc>& packet)
{
	tag_packet_from_pc result;

	tag_write_eeprom* p = reinterpret_cast<tag_write_eeprom*>(packet.get());

	result.command = p->command_id;
	result.data0 = p->cell_number;
	*(uint32_t*)result.data1 = p->value;

	return result;
}

tag_packet_from_pc logic_structures::CLogicDataFromPcToPacketFromPcConverter::convert_buttons_status(const std::shared_ptr<tag_base_data_from_pc>& packet)
{
	tag_packet_from_pc result;

	tag_buttons_status* p = reinterpret_cast<tag_buttons_status*>(packet.get());

	result.command = p->command_id;

	return result;
}
