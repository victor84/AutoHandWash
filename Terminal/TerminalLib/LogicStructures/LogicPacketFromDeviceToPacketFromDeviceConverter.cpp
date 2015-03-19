#include "stdafx.h"
#include "LogicPacketFromDeviceToPacketFromDeviceConverter.h"

using namespace logic_structures;
using namespace device_exchange;

CLogicPacketFromDeviceToPacketFromDeviceConverter::CLogicPacketFromDeviceToPacketFromDeviceConverter()
{
	fill_handlers();
}

CLogicPacketFromDeviceToPacketFromDeviceConverter::~CLogicPacketFromDeviceToPacketFromDeviceConverter()
{
}

void CLogicPacketFromDeviceToPacketFromDeviceConverter::fill_handlers()
{
	add_command_from_device_handler(e_command_from_device::button_press,			std::bind(std::mem_fn(&CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_button_press), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_device::bill_acceptor,			std::bind(std::mem_fn(&CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_bill_acceptor), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_device::hopper_issue_coin,		std::bind(std::mem_fn(&CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_hopper_issue_coin), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_device::discount_card_issued,	std::bind(std::mem_fn(&CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_discount_card_issued), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_device::data_from_eeprom,		std::bind(std::mem_fn(&CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_data_from_eeprom), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_device::buttons_state,			std::bind(std::mem_fn(&CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_buttons_state), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_device::command_confirmation,	std::bind(std::mem_fn(&CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_command_confirmation), this, std::placeholders::_1));
	add_command_from_device_handler(e_command_from_device::error,					std::bind(std::mem_fn(&CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_error), this, std::placeholders::_1));
}

device_exchange::tag_packet_from_device CLogicPacketFromDeviceToPacketFromDeviceConverter::handler_result_to_convert_result(tag_packet_from_device& result)
{
	return result;
}

tag_packet_from_device CLogicPacketFromDeviceToPacketFromDeviceConverter::get_default_result()
{
	return tag_packet_from_device();
}

tag_packet_from_device CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_button_press(const std::shared_ptr<tag_base_data_from_device>& packet)
{
	tag_packet_from_device result;

	tag_button_press* p = reinterpret_cast<tag_button_press*>(packet.get());

	result.command = p->command_id;
	result.data0 = p->button_number;

	return result;
}

tag_packet_from_device CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_bill_acceptor(const std::shared_ptr<tag_base_data_from_device>& packet)
{
	tag_packet_from_device result;

	tag_bill_acceptor* p = reinterpret_cast<tag_bill_acceptor*>(packet.get());

	result.command = p->command_id;
	*((uint16_t*)&result.data0) = p->count;

	return result;
}

tag_packet_from_device CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_hopper_issue_coin(const std::shared_ptr<tag_base_data_from_device>& packet)
{
	tag_packet_from_device result;

	tag_hopper_issue_coin* p = reinterpret_cast<tag_hopper_issue_coin*>(packet.get());

	result.command = p->command_id;
	result.data0 = p->balance;

	return result;
}

tag_packet_from_device CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_discount_card_issued(const std::shared_ptr<tag_base_data_from_device>& packet)
{
	tag_packet_from_device result;

	tag_discount_card_issued* p = reinterpret_cast<tag_discount_card_issued*>(packet.get());

	result.command = p->command_id;

	return result;
}

tag_packet_from_device CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_data_from_eeprom(const std::shared_ptr<tag_base_data_from_device>& packet)
{
	tag_packet_from_device result;

	tag_data_from_eeprom* p = reinterpret_cast<tag_data_from_eeprom*>(packet.get());

	result.command = p->command_id;
	result.data0 = p->cell_number;
	*((uint32_t*)&result.data1) = p->value;

	return result;
}

tag_packet_from_device CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_buttons_state(const std::shared_ptr<tag_base_data_from_device>& packet)
{
	tag_packet_from_device result;

	tag_buttons_state* p = reinterpret_cast<tag_buttons_state*>(packet.get());

	result.command = p->command_id;
	uint16_t state = 0;

	state |= static_cast<byte>(p->button1);
	state <<= 1;

	state |= static_cast<byte>(p->button2);
	state <<= 1;

	state |= static_cast<byte>(p->button3);
	state <<= 1;

	state |= static_cast<byte>(p->button4);
	state <<= 1;

	state |= static_cast<byte>(p->button5);
	state <<= 1;

	state |= static_cast<byte>(p->button6);
	state <<= 1;

	state |= static_cast<byte>(p->button7);
	state <<= 1;

	state |= static_cast<byte>(p->button8);
	state <<= 1;

	state |= static_cast<byte>(p->button9);
	state <<= 1;

	*((uint16_t*)&result.data0) = state;

	return result;
}

tag_packet_from_device CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_command_confirmation(const std::shared_ptr<tag_base_data_from_device>& packet)
{
	tag_packet_from_device result;

	tag_command_confirmation* p = reinterpret_cast<tag_command_confirmation*>(packet.get());

	result.command = p->command_id;
	result.data0 = static_cast<byte>(p->command);

	return result;
}

tag_packet_from_device CLogicPacketFromDeviceToPacketFromDeviceConverter::convert_error(const std::shared_ptr<tag_base_data_from_device>& packet)
{
	tag_packet_from_device result;

	tag_error* p = reinterpret_cast<tag_error*>(packet.get());

	result.command = p->command_id;
	result.data0 = static_cast<byte>(p->code);

	return result;
}
