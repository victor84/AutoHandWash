#pragma once
#include "stdafx.h"
#include "device_protocol.h"
#include "raw_data_warappers.h"

/*!
 * \file data_from_pc.h
 *
 * \author Victor Mulika
 * \date 17 Март 2015
 *
 * Структуры данных от компьютера для работы в логике терминала
 */


namespace logic_structures
{
	// базовая структура
	struct tag_base_data_from_pc
	{
		const device_exchange::e_command_from_pc command_id;	// id команды

	protected:
		tag_base_data_from_pc(device_exchange::e_command_from_pc cid) : command_id(cid)
		{

		}
	};

	// пустая команда
	struct tag_pc_empty : tag_base_data_from_pc
	{
		tag_pc_empty() : tag_base_data_from_pc(device_exchange::e_command_from_pc::empty)
		{

		}

		operator tools::data_wrappers::_tag_data_managed()
		{
			tools::data_wrappers::_tag_data_managed result;
			result.copy_data_inside(this, sizeof(*this));
			return result;
		}
	};

	// открыть клапан (ОТСЧЁТ С 1)
	struct tag_open_valve : tag_base_data_from_pc
	{
		byte number;	// номер открываемого клапана

		tag_open_valve() : tag_base_data_from_pc(device_exchange::e_command_from_pc::open_valve)
			, number(0)
		{

		}

		operator tools::data_wrappers::_tag_data_managed()
		{
			tools::data_wrappers::_tag_data_managed result;
			result.copy_data_inside(this, sizeof(*this));
			return result;
		}
	};

	// закрыть клапан (ОТСЧЁТ С 1)
	struct tag_close_valve : tag_base_data_from_pc
	{
		byte number;	// номер закрываемого клапана

		tag_close_valve() : tag_base_data_from_pc(device_exchange::e_command_from_pc::close_valve)
			, number(0)
		{

		}

		operator tools::data_wrappers::_tag_data_managed()
		{
			tools::data_wrappers::_tag_data_managed result;
			result.copy_data_inside(this, sizeof(*this));
			return result;
		}
	};

	// выдать монеты
	struct tag_issue_coins : tag_base_data_from_pc
	{
		byte number;	// количество выдаваемых монет

		tag_issue_coins() : tag_base_data_from_pc(device_exchange::e_command_from_pc::issue_coins)
			, number(0)
		{

		}

		operator tools::data_wrappers::_tag_data_managed()
		{
			tools::data_wrappers::_tag_data_managed result;
			result.copy_data_inside(this, sizeof(*this));
			return result;
		}
	};

	// выдать дисконтную карту
	struct tag_issue_discount_card : tag_base_data_from_pc
	{
		tag_issue_discount_card() : tag_base_data_from_pc(device_exchange::e_command_from_pc::issue_discount_card)
		{

		}

		operator tools::data_wrappers::_tag_data_managed()
		{
			tools::data_wrappers::_tag_data_managed result;
			result.copy_data_inside(this, sizeof(*this));
			return result;
		}
	};

	// прочитать данные eeprom
	struct tag_read_eeprom : tag_base_data_from_pc
	{
		byte cell_number;	// номер ячейки

		tag_read_eeprom() : tag_base_data_from_pc(device_exchange::e_command_from_pc::read_eeprom)
			, cell_number(0)
		{

		}

		operator tools::data_wrappers::_tag_data_managed()
		{
			tools::data_wrappers::_tag_data_managed result;
			result.copy_data_inside(this, sizeof(*this));
			return result;
		}
	};

	// записать данные eeprom
	struct tag_write_eeprom : tag_base_data_from_pc
	{
		byte		cell_number;	// номер ячейки
		uint32_t	value;			// значение

		tag_write_eeprom() : tag_base_data_from_pc(device_exchange::e_command_from_pc::write_eeprom)
			, cell_number(0)
			, value(0)
		{

		}

		operator tools::data_wrappers::_tag_data_managed()
		{
			tools::data_wrappers::_tag_data_managed result;
			result.copy_data_inside(this, sizeof(*this));
			return result;
		}
	};

	// запросить статус всех кнопок
	struct tag_buttons_status : tag_base_data_from_pc
	{
		tag_buttons_status() : tag_base_data_from_pc(device_exchange::e_command_from_pc::buttons_status)
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