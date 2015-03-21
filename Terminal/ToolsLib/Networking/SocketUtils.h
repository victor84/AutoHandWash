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
	was_connected,		// соединение было ранее установлено
	was_disconnected	// соединение было ранее разорвано
};

// параметры соединенияЫ
struct tag_connection_params
{
	std::string	address;				// адрес
	std::string	port;					// порт
	int			reconnection_timeout;	// таймаут переподключения при разрыве соединения в мс

	tag_connection_params()
		: address("")
		, port("")
		, reconnection_timeout(10000)
	{

	}
};

// состояние соединения
enum class _e_connection_state
{
	not_connected,
	connected
};

}
}
