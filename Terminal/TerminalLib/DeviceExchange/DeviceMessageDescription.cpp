#include "stdafx.h"
#include "DeviceMessageDescription.h"
#include "tools.h"


void device_exchange::CDeviceMessageDescription::fill_descriptors_storage()
{
	_device_descriptors_storage.clear();

	add_descriptor(e_command_from_device::button_press, &CDeviceMessageDescription::button_press);
	add_descriptor(e_command_from_device::bill_acceptor, &CDeviceMessageDescription::bill_acceptor);
	add_descriptor(e_command_from_device::hopper_issue_coin, &CDeviceMessageDescription::hopper_issue_coin);
	add_descriptor(e_command_from_device::discount_card_issued, &CDeviceMessageDescription::discount_card_issued);
	add_descriptor(e_command_from_device::data_from_eeprom, &CDeviceMessageDescription::data_from_eeprom);
	add_descriptor(e_command_from_device::buttons_state, &CDeviceMessageDescription::buttons_state);
	add_descriptor(e_command_from_device::command_confirmation, &CDeviceMessageDescription::command_confirmation);
	add_descriptor(e_command_from_device::error, &CDeviceMessageDescription::error);

	add_descriptor(e_command_from_pc::open_valve, &CDeviceMessageDescription::open_valve);
	add_descriptor(e_command_from_pc::close_valve, &CDeviceMessageDescription::close_valve);
	add_descriptor(e_command_from_pc::issue_coins, &CDeviceMessageDescription::issue_coins);
	add_descriptor(e_command_from_pc::issue_discount_card, &CDeviceMessageDescription::issue_discount_card);
	add_descriptor(e_command_from_pc::read_eeprom, &CDeviceMessageDescription::read_eeprom);
	add_descriptor(e_command_from_pc::write_eeprom, &CDeviceMessageDescription::write_eeprom);
	add_descriptor(e_command_from_pc::buttons_status, &CDeviceMessageDescription::buttons_status);
}

void device_exchange::CDeviceMessageDescription::add_descriptor(device_exchange::e_command_from_device command, bool(CDeviceMessageDescription::*fn)(const device_exchange::tag_packet_from_device&))
{
	_device_descriptors_storage.insert(_Device_Storage_Elem_Type(command, std::bind(std::mem_fn(fn), this, std::placeholders::_1)));
}

void device_exchange::CDeviceMessageDescription::add_descriptor(device_exchange::e_command_from_pc command, bool (CDeviceMessageDescription::*fn)(const device_exchange::tag_packet_from_pc&))
{
	_pc_descriptors_storage.insert(_Pc_Storage_Elem_Type(command, std::bind(std::mem_fn(fn), this, std::placeholders::_1)));
}

bool device_exchange::CDeviceMessageDescription::device_describe_raw(const device_exchange::tag_packet_from_device& message)
{
	_str_str.str(_T("\r\n"));

	std::wstring hex_data = tools::binary_to_hex(tools::data_wrappers::_tag_data_const((byte*)&message, sizeof(message)));
	if (0 == hex_data.length())
		return false;

	_str_str << _T("hex-������ ������ �� ����������:") << _T("\r\n");

	const wchar_t* p_hex_data = hex_data.data();
	for (size_t i = 0; i < hex_data.length(); i += 2)
	{
		_str_str << p_hex_data[i] << p_hex_data[i + 1] << _T(" ");
	}

	_str_str << _T("\r\n");

	return true;
}

bool device_exchange::CDeviceMessageDescription::button_press(const device_exchange::tag_packet_from_device& message)
{
	_str_str << _T("������ ������. ������: ") 
				<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data0, sizeof(message.data0))) 
				<< _T("\r\n")
				<< _T("����� ������: ") << message.data0;

	return true;
}

bool device_exchange::CDeviceMessageDescription::bill_acceptor(const device_exchange::tag_packet_from_device& message)
{
	_str_str << _T("������� �� ��������������� ��� ���������������.\r\n");

	return true;
}

bool device_exchange::CDeviceMessageDescription::hopper_issue_coin(const device_exchange::tag_packet_from_device& message)
{
	_str_str << _T("������ ������ ��������. �������� ������: ") << message.data0 << _T("\r\n");

	return true;
}

bool device_exchange::CDeviceMessageDescription::discount_card_issued(const device_exchange::tag_packet_from_device&)
{
	_str_str << _T("������ ���������� �����.\r\n");

	return true;
}

bool device_exchange::CDeviceMessageDescription::data_from_eeprom(const device_exchange::tag_packet_from_device& message)
{
	_str_str << _T("������ �� EEPROM. ����� ������ ") << message.data0 << _T("\r\n")
				<< _T("������: ") 
				<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data4, sizeof(message.data4)))
				<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data3, sizeof(message.data3)))
				<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data2, sizeof(message.data2)))
				<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data1, sizeof(message.data1)))
				<< _T("\r\n");

	return true;
}

bool device_exchange::CDeviceMessageDescription::buttons_state(const device_exchange::tag_packet_from_device& message)
{
	_str_str << _T("��������� ������.") << _T("\r\n");

	uint16_t all_state = *((uint16_t*)&message.data0);

	for (byte bn = 1; bn <= 9; ++bn)
	{
		_str_str << _T("������ �") << bn;
		if (true != (all_state & 0x01))
			_str_str << _T(" ��");

		_str_str << _T(" ������\r\n");
		all_state >>= 1;
	}

	return true;
}

bool device_exchange::CDeviceMessageDescription::command_confirmation(const device_exchange::tag_packet_from_device&)
{
	_str_str << _T("������������� �������.\r\n");

	return true;
}

bool device_exchange::CDeviceMessageDescription::error(const device_exchange::tag_packet_from_device& message)
{
	_str_str << _T("������. ���: ")
		<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data0, sizeof(message.data0)))
		<< _T("\r\n");

	return true;
}

bool device_exchange::CDeviceMessageDescription::pc_describe_raw(const device_exchange::tag_packet_from_pc& message)
{
	_str_str.str(_T("\r\n"));

	std::wstring hex_data = tools::binary_to_hex(tools::data_wrappers::_tag_data_const((byte*)&message, sizeof(message)));
	if (0 == hex_data.length())
		return false;

	_str_str << _T("hex-������ ������ �� pc:") << _T("\r\n");

	const wchar_t* p_hex_data = hex_data.data();
	for (size_t i = 0; i < hex_data.length(); i += 2)
	{
		_str_str << p_hex_data[i] << p_hex_data[i + 1] << _T(" ");
	}

	_str_str << _T("\r\n");

	return true;
}

bool device_exchange::CDeviceMessageDescription::open_valve(const device_exchange::tag_packet_from_pc& message)
{
	_str_str << _T("������� ������. �����: ")
		<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data0, sizeof(message.data0)))
		<< _T("\r\n");

	return true;
}

bool device_exchange::CDeviceMessageDescription::close_valve(const device_exchange::tag_packet_from_pc& message)
{
	_str_str << _T("������� ������. �����: ")
		<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data0, sizeof(message.data0)))
		<< _T("\r\n");

	return true;
}

bool device_exchange::CDeviceMessageDescription::issue_coins(const device_exchange::tag_packet_from_pc& message)
{
	_str_str << _T("������ �����: ")
		<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data0, sizeof(message.data0)))
		<< _T("\r\n");

	return true;
}

bool device_exchange::CDeviceMessageDescription::issue_discount_card(const device_exchange::tag_packet_from_pc& message)
{
	_str_str << _T("������ ���������� �����.\r\n");

	return true;
}

bool device_exchange::CDeviceMessageDescription::read_eeprom(const device_exchange::tag_packet_from_pc& message)
{
	_str_str << _T("��������� ������ EEPROM �����: ")
		<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data0, sizeof(message.data0)))
		<< _T("\r\n");

	return true;
}

bool device_exchange::CDeviceMessageDescription::write_eeprom(const device_exchange::tag_packet_from_pc& message)
{
	_str_str << _T("�������� � EEPROM. ����� ������: ") << message.data0 << _T("\r\n")
		<< _T("������: ")
		<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data4, sizeof(message.data4)))
		<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data3, sizeof(message.data3)))
		<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data2, sizeof(message.data2)))
		<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data1, sizeof(message.data1)))
		<< _T("\r\n");

	return true;
}

bool device_exchange::CDeviceMessageDescription::buttons_status(const device_exchange::tag_packet_from_pc& message)
{
	_str_str << _T("�������� ������ ���� ������.\r\n");

	return true;
}

device_exchange::CDeviceMessageDescription::CDeviceMessageDescription()
{
	_tr_error = tools::logging::CTraceError::get_instance();
	fill_descriptors_storage();
}

device_exchange::CDeviceMessageDescription::~CDeviceMessageDescription()
{
}

std::wstring device_exchange::CDeviceMessageDescription::describe_message(const device_exchange::tag_packet_from_device& message)
{
	if (false == device_describe_raw(message))
		return _str_str.str();

	_Device_Descriptor_Fn_Type descriptor = _device_descriptors_storage[message.command];

	if (!descriptor)
	{
		_str_str << _T("��� ����������� ��� ���������� ���� ���������") << std::endl;
		return _str_str.str();
	}

	_str_str << _T("���������� � ���������:") << _T("\r\n");

	if (true == descriptor(message))
		return _str_str.str();

	return _str_str.str() + _T("�� ������� ������� ���������");
}

std::wstring device_exchange::CDeviceMessageDescription::describe_message(const device_exchange::tag_packet_from_pc& message)
{
	if (false == pc_describe_raw(message))
		return _str_str.str();

	_Pc_Descriptor_Fn_Type descriptor = _pc_descriptors_storage[message.command];

	if (!descriptor)
	{
		_str_str << _T("��� ����������� ��� ���������� ���� ���������") << std::endl;
		return _str_str.str();
	}

	_str_str << _T("���������� � ���������:") << _T("\r\n");

	if (true == descriptor(message))
		return _str_str.str();

	return _str_str.str() + _T("�� ������� ������� ���������");
}
