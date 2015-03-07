#pragma once
/*!
 * \file SocketUtils.h
 *
 * \author Victor Mulika
 * \date 28 Февраль 2015
 *
 * 
 */

namespace tools
{

namespace networking
{
// Результат выполнения мотодов сокетов
enum class e_socket_result
{
	success,			// успех
	error,				// ошибка
	was_connected,		// соединение было установлено
	was_disconnected	// соединение было разорвано

};

// параметры соединенияЫ
struct tag_connection_params
{
	std::string	address;				// адрес
	std::string	port;					// порт
	int			reconnection_timeout;	// таймаут переподключения при разрыве соединения

	tag_connection_params()
		: address("")
		, port("")
		, reconnection_timeout(10000)
	{

	}
};

// статус выполнения основного рабочего цикла
enum class _e_work_loop_status
{
	ok,		// цикл продолжается
	error,	// возникла ошибка
	stop	// указали остановиться снаружи
};

// состояние соединения
enum class _e_connection_state
{
	not_connected,
	connected
};

// состояние инициализации
enum class _e_init_state
{
	not_init,	// инициализация не была произведена
	was_init	// инициализация успешна
};



}
}
