#pragma once

namespace tools
{
namespace serial_port
{

// Результат выполнения мотодов порта
enum class e_serial_result
{
	success,			// успех
	error,				// ошибка
	was_connected,		// соединение было ранее установлено
	was_disconnected,	// соединение было ранее разорвано
	no_data				// нет данных
};

// параметры подключения к COM-порту
struct tag_connection_params
{
	std::wstring	port_name;		// название порта
	uint32_t		baud_rate;		// скорость
	uint32_t		bits_per_byte;	// бит в байте
	uint32_t		stop_bits;		// количество стоп-битов

	tag_connection_params()
	{
		port_name = _T("");
		baud_rate = 9600;
		bits_per_byte = 8;
		stop_bits = 1;
	}
};
}
}