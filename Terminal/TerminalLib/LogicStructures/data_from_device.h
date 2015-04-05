#pragma once
#include "stdafx.h"
#include "device_protocol.h"
#include "raw_data_warappers.h"

/*!
 * \file data_from_device.h
 *
 * \author Victor Mulika
 * \date 17 Март 2015
 *
 * Структуры данных от устройства для работы в логике терминала
 */


namespace logic_structures
{

// базовая структура
struct tag_base_data_from_device
{
	const device_exchange::e_command_from_device command_id;	// id команды

	virtual ~tag_base_data_from_device()
	{

	}

protected:
	tag_base_data_from_device(device_exchange::e_command_from_device cid) : command_id(cid)
	{

	}
};

// пустая структура
struct tag_dev_empty : tag_base_data_from_device
{
	tag_dev_empty() : tag_base_data_from_device(device_exchange::e_command_from_device::empty)
	{

	}

	virtual ~tag_dev_empty()
	{

	}

	operator tools::data_wrappers::_tag_data_managed()
	{
		tools::data_wrappers::_tag_data_managed result;
		result.copy_data_inside(this, sizeof(*this));
		return result;
	}
};

// нажата кнопка (ОТСЧЁТ С 1)
struct tag_button_press : tag_base_data_from_device
{
	byte button_number;	// номер нажатой кнопки

	tag_button_press() 
		: tag_base_data_from_device(device_exchange::e_command_from_device::button_press)
		, button_number(0)
	{
		
	}

	virtual ~tag_button_press()
	{

	}

	operator tools::data_wrappers::_tag_data_managed()
	{
		tools::data_wrappers::_tag_data_managed result;
		result.copy_data_inside(this, sizeof(*this));
		return result;
	}
};

// данные от купюроприёмника или монетоприёмника
struct tag_bill_acceptor : tag_base_data_from_device
{
	tag_bill_acceptor() : tag_base_data_from_device(device_exchange::e_command_from_device::bill_acceptor)
	{

	}

	virtual ~tag_bill_acceptor()
	{

	}

	operator tools::data_wrappers::_tag_data_managed()
	{
		tools::data_wrappers::_tag_data_managed result;
		result.copy_data_inside(this, sizeof(*this));
		return result;
	}
};

// Выдана монета хоппером
struct tag_hopper_issue_coin : tag_base_data_from_device
{
	byte balance;	// осталось выдать

	tag_hopper_issue_coin() : tag_base_data_from_device(device_exchange::e_command_from_device::hopper_issue_coin)
		, balance(0)
	{

	}

	virtual ~tag_hopper_issue_coin()
	{

	}

	operator tools::data_wrappers::_tag_data_managed()
	{
		tools::data_wrappers::_tag_data_managed result;
		result.copy_data_inside(this, sizeof(*this));
		return result;
	}
};

// выдана дисконтная карта
struct tag_discount_card_issued : tag_base_data_from_device
{
	tag_discount_card_issued() : tag_base_data_from_device(device_exchange::e_command_from_device::discount_card_issued)
	{

	}

	virtual ~tag_discount_card_issued()
	{

	}

	operator tools::data_wrappers::_tag_data_managed()
	{
		tools::data_wrappers::_tag_data_managed result;
		result.copy_data_inside(this, sizeof(*this));
		return result;
	}
};

// данные ячейки eeprom
struct tag_data_from_eeprom : tag_base_data_from_device
{
	byte cell_number;	// номер ячейки
	uint32_t value;		// значение ячейки

	tag_data_from_eeprom() : tag_base_data_from_device(device_exchange::e_command_from_device::data_from_eeprom)
		, cell_number(0)
		, value(0)
	{

	}

	virtual ~tag_data_from_eeprom()
	{

	}

	operator tools::data_wrappers::_tag_data_managed()
	{
		tools::data_wrappers::_tag_data_managed result;
		result.copy_data_inside(this, sizeof(*this));
		return result;
	}
};

// состяние кнопок
struct tag_buttons_state : tag_base_data_from_device
{
	bool button0;	// кнопка 0
	bool button1;	// кнопка 1
	bool button2;	// кнопка 2
	bool button3;	// кнопка 3
	bool button4;	// кнопка 4
	bool button5;	// кнопка 5
	bool button6;	// кнопка 6
	bool button7;	// кнопка 7
	bool button8;	// кнопка 8


	tag_buttons_state() : tag_base_data_from_device(device_exchange::e_command_from_device::buttons_state)
		,button0(false)
		,button1(false)
		,button2(false)
		,button3(false)
		,button4(false)
		,button5(false)
		,button6(false)
		,button7(false)
		,button8(false)
	{

	}

	virtual ~tag_buttons_state()
	{

	}

	operator tools::data_wrappers::_tag_data_managed()
	{
		tools::data_wrappers::_tag_data_managed result;
		result.copy_data_inside(this, sizeof(*this));
		return result;
	}
};

// подтверждение команды
struct tag_command_confirmation : tag_base_data_from_device
{
	device_exchange::e_command_from_pc command;

	tag_command_confirmation() : tag_base_data_from_device(device_exchange::e_command_from_device::command_confirmation)
		, command(device_exchange::e_command_from_pc::empty)
	{

	}

	virtual ~tag_command_confirmation()
	{

	}

	operator tools::data_wrappers::_tag_data_managed()
	{
		tools::data_wrappers::_tag_data_managed result;
		result.copy_data_inside(this, sizeof(*this));
		return result;
	}
};

// код ошибки устройства
enum class e_device_error_code : byte
{
	error = 0x00,						// неизвестная ошибка
	empty_hopper = 0x03,				// нет денег в хоппере, DATA0 = кол - во невыданных монет
	discount_card_not_issued = 0x04,	// дисконтная карта не выдана
	error_write_eeprom = 0x0B			// ошибка записи в EEPROM
};

// ошибка
struct tag_error : tag_base_data_from_device
{
	e_device_error_code	code;	// код ошибки

	tag_error() : tag_base_data_from_device(device_exchange::e_command_from_device::error)
		, code(e_device_error_code::error)
	{

	}

	virtual ~tag_error()
	{

	}

	operator tools::data_wrappers::_tag_data_managed()
	{
		tools::data_wrappers::_tag_data_managed result;
		result.copy_data_inside(this, sizeof(*this));
		return result;
	}
};



}