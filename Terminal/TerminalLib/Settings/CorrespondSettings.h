#pragma once
#include "TraceError.h"
#include "SettingsLoader.h"
#include "ServicesNames.h"

/*!
 * \file CorrespondSettings.h
 *
 * \author Victor Mulika
 * \date 21 Март 2015
 *
 * 
 */

namespace logic_settings
{
/*
CCorrespondSettings
Возвращает соответствие одной настройке другую
*/
class CCorrespondSettings
{
	// константы чтения настроек
	const wchar_t* services_names_block_name = _T("services_names");
	const wchar_t* services_buttons_name = _T("services_buttons");
	const wchar_t* services_valves_name = _T("services_valves");
	const wchar_t* service_cost = _T("cost");

	// константы названий сервисов в настройках
	const wchar_t* service_pressurized_water = _T("pressurized_water");
	const wchar_t* service_water_without_pressure = _T("water_without_pressure");
	const wchar_t* service_foam = _T("foam");
	const wchar_t* service_wax = _T("wax");
	const wchar_t* service_against_midges = _T("against_midges");
	const wchar_t* service_vacuum_cleaner = _T("vacuum_cleaner");
	const wchar_t* service_air = _T("air");
	const wchar_t* service_osmosis = _T("osmosis");
	const wchar_t* service_stop = _T("stop");

	tools::logging::CTraceError* _tr_error;
	tools::settings::CSettingsLoader* _settings_module;

	// соответствие сервис - название сервиса в настройках
	std::map<logic::e_service_name, const wchar_t*> _service_settings_name;

	// соответствие сервис-название сервиса
	std::map<logic::e_service_name, std::wstring> _service_service_name;

	// соответствие сервис-номер кнопки
	std::map<logic::e_service_name, byte> _service_button_number;

	// соответствие номер кнопки-сервис
	std::map<byte, logic::e_service_name> _button_number_service;

	// соответствие сервис-номер клапана
	std::map<logic::e_service_name, byte> _service_valve_number;

	// соответствие номер кнопки - номер клапана
	std::map<byte, byte> _button_number_valve_number;

	// соответствие сервис - стоимость
	std::map<logic::e_service_name, uint16_t> _service_cost;

	// признак отсутствия ошибки во время чтения настроек
	bool _read_settings_success;

	// проверка, что чтение настроек было успешно
	bool is_reading_successed();

	void fill_service_settings_name();

	void fill_parameters_for_read();

	bool fill_service_service_name();

	bool fill_services_buttons();

	bool fill_buttons_services();

	bool fill_services_valves();

	bool fill_button_number_valve_number();

	bool fill_services_cost();

public:
	CCorrespondSettings();
	virtual ~CCorrespondSettings();

	// Чтение настроек из файла
	bool ReadSettings();

	// получить название сервиса
	std::wstring GetServiceName(const logic::e_service_name& service);

	// получить номер кнопки
	byte GetButtonNumber(const logic::e_service_name& service);

	// получить номер клапана
	byte GetValveNumber(const logic::e_service_name& service);

	// получить номер клапана по номеру кнопки
	byte GetValveNumber(const byte& button_number);

	// получить сервис по номеру кнопки
	logic::e_service_name GetServiceByButtonNumber(const byte& button_number);

	// получить стоимость услуги
	uint16_t GetServiceCost(const logic::e_service_name& service);

	// установить и записать стоимость услуги
	bool SetServiceCost(const logic::e_service_name& service, const uint16_t& cost);

};
}
