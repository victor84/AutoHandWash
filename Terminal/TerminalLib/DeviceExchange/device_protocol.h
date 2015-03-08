#pragma once
#include "stdafx.h"
/*!
 * \file device_protocol.h
 *
 * \author Victor Mulika
 * \date 03 Март 2015
 *
 * Структуры обмена с устройством
 *
 */

namespace device_exchange
{

/******************************************************************************************
Формат хранения данных в EEPROM контроллера
Общая память = 256 байт / 4 байта на запись = 64 ячейки: 0x00 – 0x3F
0x00 – вход всех денег
0x01 – текущий баланс
0x02 – импульс купюроприемника
0x03 – импульс монетоприемника
0x04 – время простоя
0x05 – стоимость 1 минуты простоя
0x06 – пауза перед показом рекламы после обнуления баланса
0x11 – 0x18 стоимость минуты каждого клапана
0x21 – 0x28 учет секунд работы каждого клапана
******************************************************************************************/

/******************************************************************************************
Контроллер будет автоматически при вводе купюры или опускании монеты в монетоприемник 
отправлять команду 0x02 со суммой которую необходимо добавить к кредиту. 
Сумма будет браться из ячек 0x02 и 0x03 соответственно. На ячейки 0x00 и 0x01 
это не будет влиять.
PC должен сам записать в них свои данные
******************************************************************************************/

// команды от устройства
enum class e_command_from_device : byte
{
	// пустая команда
	empty = 0x00,

	// CMD=0x01 Нажата кнопка, DATA0 = 0x01… 0x08
	button_press = 0x01,

	// CMD=0x02 Данные от купюроприемника или монетоприемника
	// DATA0 = сумма, которую нужно добавить к кредиту
	bill_acceptor = 0x02,

	// CMD=0x03 Выдана монета хоппером. Осталось выдать DATA0
	hopper_issue_coin = 0x03,

	// CMD=0x04 Выдана дисконтная карта
	discount_card_issued = 0x04,

	// CMD=0x0A Передать данные из EEPROM, DATA0=номер ячейки,
	// DATA1, DATA2, DATA3, DATA4 = 4 байта ячейки DATA1 младший, DATA4 старший
	data_from_eeprom = 0x0a,

	// CMD=0x0C Выдача статуса всех кнопок. DATA0=8бит, соответствующих каждой кнопке.
	// 7й бит = кнопка8, 0й бит = кнопка1, 1 = кнопка нажата.
	buttons_state = 0x0c,

	// CMD=0x0E Подтверждение выполнения команды, DATA0=код команды которой 
	// выдается подтверждение. Отправляется только в ответ команде от PC
	command_confirmation = 0x0e,

	// CMD=0x0F Ошибка, DATA0=код ошибки:
	// 0x03 – нет денег в хоппере, DATA0 = кол - во невыданных монет
	// 0x04 – дисконтная карта не выдана
	// 0x0B – ошибка записи в EEPROM
	error = 0x0f

};

// команды от pc
enum class e_command_from_pc : byte
{
	// пустая команда
	empty = 0x00,

	// CMD=0x10 Включить клапан. DATA0=0x01…0x08 – номер клапана
	open_valve = 0x10,

	// CMD=0x20 Выключить клапан. DATA0=0x01…0x08 – номер клапана
	close_valve = 0x20,

	// CMD=0x30 Выдать хоппером монеты. DATA0=кол-во монет. 
	// В ответ будет приходить команда 0x03 с кол-вом оставшихся монет.
	issue_coins = 0x30,

	// CMD=0x40 Выдать дисконтную карту.
	issue_discount_card = 0x40,

	// CMD=0xA0 Считать данные из EEPROM, DATA0=номер ячейки
	read_eeprom = 0xa0,

	// CMD=0xB0 Записать данные из EEPROM, DATA0=номер ячейки,
	// DATA1, DATA2, DATA3, DATA4 = 4 байта ячейки DATA1 младший, DATA4 старший
	write_eeprom = 0xb0,

	// CMD=0xC0 Чтение статуса всех кнопок.
	buttons_status
};

/******************************************************************************************
Формат посылки:
0x0A CMD DATA0 DATA1 DATA2 DATA3 DATA4 DATA5 CRC 0xA0, где
0x0A – начало пакета
CMD – команда
DATA0 – данные0
DATA1 – данные1
DATA2 – данные2
DATA3 – данные3
DATA4 – данные4
DATA5 – данные5
CRC – контрольная сумма = CMD xor DATA0 xor DATA1 xor DATA2 xor DATA3 xor DATA4 xor DATA5
0xA0 – конец пакета
******************************************************************************************/

// начальный байт
const byte begin_byte = 0x0a;

// завершающий байт
const byte end_byte = 0xa0;

#pragma pack(push, 1)
// команда от устройства
struct tag_packet_from_device
{
	byte begin;						// 0x0A – начало пакета
	e_command_from_device command;	// CMD – команда
	byte data0;						// данные0
	byte data1;						// данные1
	byte data2;						// данные2
	byte data3;						// данные3
	byte data4;						// данные4
	byte data5;						// данные5
	byte crc;						// CRC – контрольная сумма = CMD xor DATA0 xor DATA1 xor DATA2 xor DATA3 xor DATA4 xor DATA5
	byte end;						// конец пакета

	tag_packet_from_device()
	{
		clear();
	}

	void clear()
	{
		begin	= begin_byte;
		command	= e_command_from_device::empty;
		data0	= 0;
		data1	= 0;
		data2	= 0;
		data3	= 0;
		data4	= 0;
		data5	= 0;
		crc		= 0;
		end		= end_byte;
	}
};

// команда от pc
struct tag_packet_from_pc
{
	byte begin;					// 0x0A – начало пакета
	e_command_from_pc command;	// CMD – команда
	byte data0;					// данные0
	byte data1;					// данные1
	byte data2;					// данные2
	byte data3;					// данные3
	byte data4;					// данные4
	byte data5;					// данные5
	byte crc;					// CRC – контрольная сумма = CMD xor DATA0 xor DATA1 xor DATA2 xor DATA3 xor DATA4 xor DATA5
	byte end;					// конец пакета

	tag_packet_from_pc()
	{
		clear();
	}

	void clear()
	{
		begin = begin_byte;
		command = e_command_from_pc::empty;
		data0 = 0;
		data1 = 0;
		data2 = 0;
		data3 = 0;
		data4 = 0;
		data5 = 0;
		crc = 0;
		end = end_byte;
	}
};
#pragma pack(pop)

// результат преобразования данных
enum class e_convert_result
{
	invalid_data,	// неверные данные
	success,		// успех
	empty_data		// пустой блок данных
};

}



