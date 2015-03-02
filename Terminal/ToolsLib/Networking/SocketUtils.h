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
};
}
}
