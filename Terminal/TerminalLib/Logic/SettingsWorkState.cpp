#include "stdafx.h"
#include "SettingsWorkState.h"
#include "AdvertisingIdleState.h"


logic::CSettingsWorkState::CSettingsWorkState(CLogicAbstract& logic)
	: IState(logic, e_state::settings_work)
	, _p_settings(reinterpret_cast<uint32_t*>(&_settings))
	, _write_cell_number(0x00)
	, _write_setting_number(0x00)
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

logic::CSettingsWorkState::~CSettingsWorkState()
{
}

void logic::CSettingsWorkState::refilled_cache()
{

}

void logic::CSettingsWorkState::service_button_press(e_service_name service_name)
{

}

void logic::CSettingsWorkState::stop_button_press()
{

}

void logic::CSettingsWorkState::time_out()
{

}

void logic::CSettingsWorkState::out_of_money()
{

}

void logic::CSettingsWorkState::device_confirm()
{
	uint32_t* p_ds = reinterpret_cast<uint32_t*>(&_settings_from_device);
	bool continue_loop = true;

	while (true == continue_loop)
	{
		if ((_write_setting_number >= (sizeof(tag_device_settings) / sizeof(uint32_t))) ||
			(_write_cell_number > 0x28))
		{
			_settings_from_device = _settings;
			// запись закончена
			_logic.set_state(e_state::advertising_idle);
			CAdvertisingIdleState* as = get_implemented_state<CAdvertisingIdleState>(e_state::advertising_idle);
			as->time_out();
			return;
		}

		if ((*(_p_settings + _write_setting_number)) != (*(p_ds + _write_setting_number)))
		{
			_logic.write_eeprom(_write_cell_number, *(_p_settings + _write_setting_number));
			continue_loop = false;
		}

		if (0x07 == _write_cell_number)
			_write_cell_number = 0x10;

		if (0x18 == _write_cell_number)
			_write_cell_number = 0x20;

		++_write_cell_number;
		++_write_setting_number;
	}
}

void logic::CSettingsWorkState::data_from_eeprom(byte cell_number, uint32_t value)
{
	if (cell_number < 0x08)
	{
		*(_p_settings + cell_number) = value;

		++cell_number;

		if (cell_number > 0x07)
			cell_number = 0x11;
	}
	else if ((cell_number >= 0x11) && (cell_number <= 0x18))
	{
		*(_p_settings + (cell_number - 0x09)) = value;

		++cell_number;

		if (cell_number > 0x18)
			cell_number = 0x21;
	}
	else if ((cell_number >= 0x21) && (cell_number <= 0x28))
	{
		*(_p_settings + (cell_number - 0x09 - 0x08)) = value;

		++cell_number;

		if (cell_number > 0x28)
		{
			// чтение закончено
			_settings_from_device = _settings;
			_logic.set_state(e_state::advertising_idle);
			CAdvertisingIdleState* as = get_implemented_state<CAdvertisingIdleState>(e_state::advertising_idle);
			as->time_out();
			_logic.on_settings_readed();
			return;
		}
	}
	else
	{
		_str_str.str(std::wstring());
		_str_str << _T("Данные от неизвестной ячейки памяти ") << std::hex << std::setw(2) 
					<< cell_number << std::endl;

		_tr_error->trace_error(_str_str.str());
		return;
	}
	_logic.read_eeprom(cell_number);
}

void logic::CSettingsWorkState::read_settings()
{
	_settings.clear();

	_logic.read_eeprom(0x00);
}

logic::tag_device_settings logic::CSettingsWorkState::get_settings() const
{
	return _settings;
}

void logic::CSettingsWorkState::set_settings(const tag_device_settings& settings)
{
	_settings = settings;
}

void logic::CSettingsWorkState::write_settings()
{
	_write_cell_number = 0x00;
	_write_setting_number = 0x00;

	_logic.write_eeprom(_write_cell_number, *_p_settings);
}
