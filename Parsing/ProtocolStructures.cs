﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Parsing
{
// результат преобразования данных
public enum e_convert_result
{
	invalid_data,	// неверные данные
	success,		// успех
	empty_data		// пустой блок данных
};

public struct tag_constants
{
    // начало пакета
    public const UInt16 begin_bytes = 0xf00d;

    // конец пакета
    public const UInt16 end_bytes = 0x1013;
}

// тип пакета
public enum e_packet_type : byte
{
	unknown = 0,	// неизвестно
	id = 1,			// пакет идентификации терминала
	counters,		// показания счётчиков
	settings,		// настройки
    confirmation,   // подтверждение
    log             // лог
};

/******************************************************************************************
Структура транспортного пакета
-------------------------------------------------------------------------------------
Название						|	Длина	|	Тип		|	Фиксированное значение	|
-------------------------------------------------------------------------------------
Начало пакета					|	2		|	USHORT	|			0xF00D			|
Тип пакета						|	1		|	BYTE	|							|
Длина данных					|	2		|	USHORT	|							|
Данные							|1...65536	| 	ARRAY	|							|
Окончание пакета				|	2		|	USHORT	|			0x1013			|
-------------------------------------------------------------------------------------
******************************************************************************************/
// транспортный пакет, в котором передаются данные
public struct tag_transport_packet 
{
    public UInt16 begin;		// начальные байты 0x0d и 0xf0
    public e_packet_type type;	// тип пакета
    public UInt32 length;		// длина данных (без учёта последних байт)
    public Byte[] data;		    // даннные
    public UInt16 end;		    // окончание пакета

    // устанавливает значения недостающих полей
    // предварительно должны быть установлены поле данных и тип пакета
    public void set_missing_values()
    {
        begin = tag_constants.begin_bytes;
        end = tag_constants.end_bytes;
        length = (UInt32)data.Length;
    }

};

/******************************************************************************************
Структура пакета идентификации
-------------------------------------------------------------------------------------
Название						|	Длина	|	Тип		|	Фиксированное значение	|
-------------------------------------------------------------------------------------
Название группы					|	20		|CHAR ARRAY	|							|
Название терминала				|	20		|CHAR ARRAY	|							|
-------------------------------------------------------------------------------------
******************************************************************************************/

public struct tag_identification_packet
{
	public char[]	group_name;		// название группы терминалов
	public char[]	terminal_name;	// название терминала
};


/******************************************************************************************
Структура пакета с показаниями счётчиков
-------------------------------------------------------------------------------------
Название						|	Длина	|	Тип		|	Фиксированное значение	|
-------------------------------------------------------------------------------------
Дата и время					|	  8		|ULONGLONG	|							|
Общий вход						|	  4		|	UINT	|							|
Текущий баланс					|	  4		|	UINT	|							|
Состояние (работает, простой	|	  1		|	BYTE	|							|
сломана)						-----------------------------------------------------
Время работы услуг:				-----------------------------------------------------
Вода по давлением				|	  4		|	UINT	|							|
Вода без давления				|	  4		|	UINT	|							|
Пена							|	  4		|	UINT	|							|
Воск							|	  4		|	UINT	|							|
От мошек						|	  4		|	UINT	|							|
Пылесос							|	  4		|	UINT	|							|
Воздух							|	  4		|	UINT	|							|
Осмос							|	  4		|	UINT	|							|
-------------------------------------------------------------------------------------
******************************************************************************************/

// состояние терминала
public enum e_terminal_state : byte
{
	work = 0,	// терминал в работе
	idle = 1,	// простаивает
	broken = 2,	// сломан
    blocked = 3	// заблокирован
};

// пакет с показаниями счётчиков
public struct tag_counters_packet
{
    public UInt64           date_time;              // Дата и время сообщения
    public UInt32           total_cache;			// Общий вход
    public UInt32           current_cache;			// Текущий баланс
    public e_terminal_state state;			        // Состояние (работает, простой, сломана)
    public UInt32           pressurized_water;		// Вода по давлением
    public UInt32           water_without_pressure;	// Вода без давления
    public UInt32           foam;					// Пена	
    public UInt32           wax;					// Воск	
    public UInt32           against_midges;			// От мошек
    public UInt32           vacuum_cleaner;			// Пылесос	
    public UInt32           air;					// Воздух
    public UInt32           osmosis;                // Осмос
};

/******************************************************************************************
Структура пакета с настройками
-------------------------------------------------------------------------------------
Название						|	Длина	|	Тип		|	Фиксированное значение	|
-------------------------------------------------------------------------------------
Состояние (работает, простой	|	  1		|	BYTE	|							|
сломана)						-----------------------------------------------------
Импульс купюроприёмника			|	  1		|	BYTE	|							|
Импульс монетоприёмника			|	  1		|	BYTE	|							|
Время простоя					|	  1		|	BYTE	|							|
Стоимость минуты простоя		|	  2		|	USHORT	|							|
Пауза перед показом рекламы		|	  1		|	BYTE	|							|
Стоимость работы услуг:			-----------------------------------------------------
Вода по давлением				|	  4		|	UINT	|							|
Вода без давления				|	  4		|	UINT	|							|
Пена							|	  4		|	UINT	|							|
Воск							|	  4		|	UINT	|							|
От мошек						|	  4		|	UINT	|							|
Пылесос							|	  4		|	UINT	|							|
Воздух							|	  4		|	UINT	|							|
Осмос							|	  4		|	UINT	|							|
-------------------------------------------------------------------------------------
******************************************************************************************/

public struct tag_settings_packet
{
    public e_terminal_state state;			// Состояние (работает, простой, сломана)
    public byte bill_acceptor_impulse;		// Импульс купюроприёмника		
    public byte coin_acceptor_impulse;		// Импульс монетоприёмника		
    public byte free_idle_time;				// Время простоя				
    public UInt16 idle_time_cost;			// Стоимость минуты простоя	
    public byte pause_before_advertising;	// Пауза перед показом рекламы
    public UInt32 pressurized_water;		// Вода по давлением
    public UInt32 water_without_pressure;	// Вода без давления
    public UInt32 foam;						// Пена	
    public UInt32 wax;						// Воск	
    public UInt32 against_midges;			// От мошек
    public UInt32 vacuum_cleaner;			// Пылесос	
    public UInt32 air;						// Воздух	
    public UInt32 osmosis;					// Осмос
};

/******************************************************************************************
Структура пакета подтверждения
-------------------------------------------------------------------------------------
Название						|	Длина	|	Тип		|	Фиксированное значение	|
-------------------------------------------------------------------------------------
Тип пакета						|	1		|	BYTE	|							|
Статус обработки				|	1		|	BYTE	|							|
-------------------------------------------------------------------------------------
******************************************************************************************/

// результат обработки пакета
public enum e_processing_result : byte
{
	success = 0,	// пакет успешно обработан
	failed			// не удалось обработать пакет
};

public struct tag_confirmation_packet
{
    public e_packet_type packet_type;	// тип пакета
    public e_processing_result result;	// результат обработки
};

/******************************************************************************************
Структура пакета записи лога
-------------------------------------------------------------------------------------
Название						|	Длина	|	Тип		|	Фиксированное значение	|
-------------------------------------------------------------------------------------
Дата							|	8		|	ULONG	|							|
Тип записи						|	1		|	BYTE	|							|
Длина записи					|	2		|	USHORT	|							|
Текст записи					|	1-65536	|CHAR ARRAY	|							|
-------------------------------------------------------------------------------------
******************************************************************************************/

// тип записи лога
public enum e_log_record_type : byte
{
	error = 0,		// ошибка
	warning = 1,	// предупреждение
	message = 2		// сообщение
};

// запись лога
public struct tag_log_record_packet
{
    public UInt64 date_time;	    // Дата		
    public e_log_record_type type;	// Тип записи	
    public UInt16 length;		    // Длина записи
    public Char[] text;		        // Текст записи
};

}
