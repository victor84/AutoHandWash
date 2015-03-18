#include "stdafx.h"
#include "DeveiceMessageDescription.h"
#include "tools.h"

using namespace device_exchange;

void CDeveiceMessageDescription::fill_descriptors_storage()
{
	_descriptors_storage.clear();

	add_descriptor(e_command_from_device::button_press, &CDeveiceMessageDescription::button_press);
	add_descriptor(e_command_from_device::bill_acceptor, &CDeveiceMessageDescription::bill_acceptor);
	add_descriptor(e_command_from_device::hopper_issue_coin, &CDeveiceMessageDescription::hopper_issue_coin);
	add_descriptor(e_command_from_device::discount_card_issued, &CDeveiceMessageDescription::discount_card_issued);
	add_descriptor(e_command_from_device::data_from_eeprom, &CDeveiceMessageDescription::data_from_eeprom);
	add_descriptor(e_command_from_device::buttons_state, &CDeveiceMessageDescription::buttons_state);
	add_descriptor(e_command_from_device::command_confirmation, &CDeveiceMessageDescription::command_confirmation);
	add_descriptor(e_command_from_device::error, &CDeveiceMessageDescription::error);
}

void CDeveiceMessageDescription::add_descriptor(device_exchange::e_command_from_device command, bool(CDeveiceMessageDescription::*fn)(const device_exchange::tag_packet_from_device&))
{
	_descriptors_storage.insert(_Storage_Elem_Type(command, std::bind(std::mem_fn(fn), this, std::placeholders::_1)));
}

bool CDeveiceMessageDescription::describe_raw(const device_exchange::tag_packet_from_device& message)
{
	_str_str.str(_T("\r\n"));

	std::wstring hex_data = tools::binary_to_hex(tools::data_wrappers::_tag_data_const((byte*)&message, sizeof(message)));
	if (0 == hex_data.length())
		return false;

	_str_str << _T("hex-данные пакета:") << _T("\r\n");

	const wchar_t* p_hex_data = hex_data.data();
	for (size_t i = 0; i < hex_data.length(); i += 2)
	{
		_str_str << p_hex_data[i] << p_hex_data[i + 1] << _T(" ");
	}

	_str_str << _T("\r\n");

	return true;
}

bool CDeveiceMessageDescription::button_press(const device_exchange::tag_packet_from_device& message)
{
	_str_str << _T("Нажата кнопка. Данные: ") 
				<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data0, sizeof(message.data0))) 
				<< _T("\r\n");

	byte button_num = 0;
	byte data = message.data0;
	for (; button_num < 8; ++button_num)
	{
		if (data & 0x01)
			break;
		data >>= 1;
	}

	if (8 == button_num)
	{
		_str_str << _T("Ни одна кнопка не нажата") << _T("\r\n");
	}
	else
	{
		_str_str << _T("Номер кнопки: ") << ++button_num << _T("\r\n");
	}

	return true;
}

bool CDeveiceMessageDescription::bill_acceptor(const device_exchange::tag_packet_from_device& message)
{
	_str_str << _T("Данные от купюроприемника или монетоприемника. Сумма: ") 
				<< _byteswap_ushort(*((uint16_t*)&message.data0))
				<< _T("\r\n");

	return true;
}

bool CDeveiceMessageDescription::hopper_issue_coin(const device_exchange::tag_packet_from_device& message)
{
	_str_str << _T("Выдана монета хоппером. Осталось выдать: ") << message.data0 << _T("\r\n");

	return true;
}

bool CDeveiceMessageDescription::discount_card_issued(const device_exchange::tag_packet_from_device&)
{
	_str_str << _T("Выдана дисконтная карта.\r\n");

	return true;
}

bool CDeveiceMessageDescription::data_from_eeprom(const device_exchange::tag_packet_from_device& message)
{
	_str_str << _T("Данные из EEPROM. Номер ячейки ") << message.data0 << _T("\r\n")
				<< _T("Данные: ") 
				<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data4, sizeof(message.data4)))
				<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data3, sizeof(message.data3)))
				<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data2, sizeof(message.data2)))
				<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data1, sizeof(message.data1)))
				<< _T("\r\n");

	return true;
}

bool CDeveiceMessageDescription::buttons_state(const device_exchange::tag_packet_from_device& message)
{
	_str_str << _T("Состояние кнопок.") << _T("\r\n");

	uint16_t all_state = *((uint16_t*)&message.data0);

	for (byte bn = 1; bn <= 9; ++bn)
	{
		_str_str << _T("Кнопка №") << bn;
		if (true != (all_state & 0x01))
			_str_str << _T(" не");

		_str_str << _T(" нажата\r\n");
		all_state >>= 1;
	}

	return true;
}

bool CDeveiceMessageDescription::command_confirmation(const device_exchange::tag_packet_from_device&)
{
	_str_str << _T("Подтверждение команды.\r\n");

	return true;
}

bool CDeveiceMessageDescription::error(const device_exchange::tag_packet_from_device& message)
{
	_str_str << _T("Ошибка. Код: ")
		<< tools::binary_to_hex(tools::data_wrappers::_tag_data_const(&message.data0, sizeof(message.data0)))
		<< _T("\r\n");

	return true;
}

CDeveiceMessageDescription::CDeveiceMessageDescription()
{
	_tr_error = tools::logging::CTraceError::get_instance();
	fill_descriptors_storage();
}

CDeveiceMessageDescription::~CDeveiceMessageDescription()
{
}

std::wstring CDeveiceMessageDescription::describe_message(const device_exchange::tag_packet_from_device& message)
{
	if (false == describe_raw(message))
		return _str_str.str();

	_Descriptor_Fn_Type descriptor = _descriptors_storage[message.command];

	if (!descriptor)
	{
		_str_str << _T("Нет обработчика для указанного типа сообщения") << std::endl;
		return _str_str.str();
	}

	_str_str << _T("Информация о сообщении:") << _T("\r\n");

	if (true == descriptor(message))
		return _str_str.str();

	return _str_str.str() + _T("Не удалось описать сообщение");
}
