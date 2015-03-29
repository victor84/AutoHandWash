#include "stdafx.h"
#include "SettingsWorkState.h"


logic::CSettingsWorkState::CSettingsWorkState(CLogicAbstract& logic)
	: IState(logic, e_state::settings_work)
	, _p_settings(reinterpret_cast<uint32_t*>(&_settings))
	, _write_cell_number(0x00)
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

logic::CSettingsWorkState::~CSettingsWorkState()
{
}

void logic::CSettingsWorkState::refilled_cache(uint16_t cache)
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
	byte offset = 0;
	++_write_cell_number;

	if (_write_cell_number <= 0x07)
	{

	}
	else if ((_write_cell_number > 0x07) && (_write_cell_number <= 0x18))
	{
		offset += 4;
	}
	else if ((_write_cell_number > 0x18) && (_write_cell_number <= 0x28))
	{
		offset += 4 + 3;
	}
	else
	{
		// запись закончена
		_logic.set_state(e_state::advertising_idle);
	}

	_logic.write_eeprom(_write_cell_number + offset, *_p_settings);
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
			_logic.set_state(e_state::advertising_idle);
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

void logic::CSettingsWorkState::set_settings(tag_device_settings settings)
{
	_settings = settings;
}

void logic::CSettingsWorkState::write_settings()
{
	_write_cell_number = 0x00;

	_logic.write_eeprom(_write_cell_number, *_p_settings);
}
