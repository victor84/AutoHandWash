#include "stdafx.h"
#include "DeveiceMessageDescription.h"
#include "tools.h"

using namespace device_exchange;

void CDeveiceMessageDescription::fill_descriptors_storage()
{
	_descriptors_storage.clear();

	_descriptors_storage.insert(_Storage_Elem_Type(e_command_from_device::button_press, 
		std::bind(std::mem_fn(&CDeveiceMessageDescription::button_press), this, std::placeholders::_1)));


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
