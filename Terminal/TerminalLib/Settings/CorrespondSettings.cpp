#include "stdafx.h"
#include "CorrespondSettings.h"

using namespace logic;

bool logic_settings::CCorrespondSettings::is_reading_successed()
{
	if (false == _read_settings_success)
		_tr_error->trace_error(_T("Чтение настроек ошибочно, либо не было произведено"));

	return _read_settings_success;
}

void logic_settings::CCorrespondSettings::fill_service_settings_name()
{
	_service_settings_name.clear();

	_service_settings_name.insert(std::pair<e_service_name, const wchar_t*>
								  (e_service_name::pressurized_water, service_pressurized_water));
	_service_settings_name.insert(std::pair<e_service_name, const wchar_t*>
								  (e_service_name::water_without_pressure, service_water_without_pressure));
	_service_settings_name.insert(std::pair<e_service_name, const wchar_t*>
								  (e_service_name::foam, service_foam));
	_service_settings_name.insert(std::pair<e_service_name, const wchar_t*>
								  (e_service_name::wax, service_wax));
	_service_settings_name.insert(std::pair<e_service_name, const wchar_t*>
								  (e_service_name::against_midges, service_against_midges));
	_service_settings_name.insert(std::pair<e_service_name, const wchar_t*>
								  (e_service_name::vacuum_cleaner, service_vacuum_cleaner));
	_service_settings_name.insert(std::pair<e_service_name, const wchar_t*>
								  (e_service_name::air, service_air));
	_service_settings_name.insert(std::pair<e_service_name, const wchar_t*>
								  (e_service_name::osmosis, service_osmosis));
}

void logic_settings::CCorrespondSettings::fill_parameters_for_read()
{
	for (int i = 0; i < 8; ++i)
	{
		e_service_name service = static_cast<e_service_name>(i);
		_settings_module->add_parameter(tools::settings::CSettingsLoader::e_parameter_data_type::type_string,
										services_names_block_name,
										_service_settings_name[service]);

		_settings_module->add_parameter(tools::settings::CSettingsLoader::e_parameter_data_type::type_int,
										services_buttons_name,
										_service_settings_name[service]);

		_settings_module->add_parameter(tools::settings::CSettingsLoader::e_parameter_data_type::type_int,
										services_valves_name,
										_service_settings_name[service]);
	}
}

bool logic_settings::CCorrespondSettings::fill_service_service_name()
{
	_service_service_name.clear();

	typedef std::pair<logic::e_service_name, std::wstring> _ElemType;

	for (int i = 0; i < 8; ++i)
	{
		e_service_name service = static_cast<e_service_name>(i);
		std::wstring service_name = _settings_module->get_string(_service_settings_name[service], services_names_block_name).GetString();

		if (_service_service_name[service] != std::wstring())
		{
			_tr_error->trace_error(_T("Ошибка в блоке названий сервисов"));
			return false;
		}

		_service_service_name.insert(_ElemType(service, service_name));
	}
	return true;
}

bool logic_settings::CCorrespondSettings::fill_services_buttons()
{
	_service_button_number.clear();

	typedef std::pair<logic::e_service_name, byte> _ElemType;

	for (int i = 0; i < 8; ++i)
	{
		e_service_name service = static_cast<e_service_name>(i);
		byte button_number = static_cast<byte>(_settings_module->get_int(_service_settings_name[service], services_buttons_name));

		if (_service_button_number[service] != 0)
		{
			_tr_error->trace_error(_T("Ошибка в блоке сервис-номер кнопки"));
			return false;
		}

		_service_button_number.insert(_ElemType(service, button_number));
	}
	return true;
}

bool logic_settings::CCorrespondSettings::fill_services_valves()
{
	_service_valve_number.clear();

	typedef std::pair<logic::e_service_name, byte> _ElemType;

	for (int i = 0; i < 8; ++i)
	{
		e_service_name service = static_cast<e_service_name>(i);
		byte valve_number = static_cast<byte>(_settings_module->get_int(_service_settings_name[service], services_valves_name));

		if (_service_button_number[service] != 0)
		{
			_tr_error->trace_error(_T("Ошибка в блоке сервис-номер клапана"));
			return false;
		}

		_service_valve_number.insert(_ElemType(service, valve_number));
	}
	return true;
}

bool logic_settings::CCorrespondSettings::fill_button_number_valve_number()
{
	_button_number_valve_number.clear();

	for (int i = 0; i < 8; ++i)
	{
		e_service_name service = static_cast<e_service_name>(i);
		byte button_number = static_cast<byte>(_settings_module->get_int(_service_settings_name[service], services_buttons_name));
		byte valve_number = static_cast<byte>(_settings_module->get_int(_service_settings_name[service], services_valves_name));

		_button_number_valve_number.insert(std::pair<byte, byte>(button_number, valve_number));
	}

	return true;
}

logic_settings::CCorrespondSettings::CCorrespondSettings()
	: _read_settings_success(false)
{
	_tr_error = tools::logging::CTraceError::get_instance();
	_settings_module = tools::settings::CSettingsLoader::get_instance();

	fill_service_settings_name();
	fill_parameters_for_read();
}

logic_settings::CCorrespondSettings::~CCorrespondSettings()
{
}

bool logic_settings::CCorrespondSettings::ReadSettings()
{
	_read_settings_success = false;

	if (FALSE == _settings_module->read_all())
		return false;

	if (false == fill_service_service_name())
		return false;

	if (false == fill_services_buttons())
		return false;

	if (false == fill_services_valves())
		return false;

	if (false == fill_button_number_valve_number())
		return false;

	_read_settings_success = true;

	return true;
}

std::wstring logic_settings::CCorrespondSettings::GetServiceName(logic::e_service_name service)
{
	if (false == is_reading_successed())
		return std::wstring();

	return _service_service_name[service];
}

byte logic_settings::CCorrespondSettings::GetButtonNumber(logic::e_service_name service)
{
	if (false == is_reading_successed())
		return 0;

	return _service_button_number[service];
}

byte logic_settings::CCorrespondSettings::GetValveNumber(logic::e_service_name service)
{
	if (false == is_reading_successed())
		return 0;

	return _service_valve_number[service];
}

byte logic_settings::CCorrespondSettings::GetValveNumber(byte button_number)
{
	if (false == is_reading_successed())
		return 0;

	return _button_number_valve_number[button_number];
}
