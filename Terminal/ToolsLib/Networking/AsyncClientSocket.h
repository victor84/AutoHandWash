#pragma once
#include "TraceError.h"
#include "SocketUtils.h"
#include "raw_data_warappers.h"
#include "lock_vector.h"
#include "SocketStream.h"

/*!
 * \file AsyncClientSocket.h
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
// Асинхронный сокет клиент
class CAsyncClientSocket
{
	tools::logging::CTraceError* _tr_error;

	SOCKET _client_socket;

	CSocketStream _socket_stream;

	// работа с текстами ошибок
	std::wstringstream _str_str;

	addrinfo	_addr_hints;
	addrinfo	*_addr_results;
	addrinfo	*_addrptr;

	// результат выполнения функции windows socket
	INT _socket_fn_result;

	// параметры подключения
	tag_connection_params _connection_params;

	// состояние подключения
	_e_connection_state _connection_state;

	// состояние инициализации
	tools::e_init_state _init_state;

	// статус обработки данных
	e_work_loop_status _end_of_stream_status;

	// инициализация сокета
	e_socket_result init();

	void cleanup();

	// true если _socket_fn_result равен valid_val
	bool check_socket_fn_result_and(const INT& valid_val);

	// true если _socket_fn_result не равен invalid_val
	bool check_socket_fn_result_not(const INT& invalid_val);

	// таймер переподключения
	Concurrency::timer<INT>* _reconnection_timer;

	// метод переподключения
	Concurrency::call<INT> _reconnection_method;

	// вызывается при установке соединения до приёма данных
	std::function<void(void)> _on_connected;

	// вызывается при разрыве соединения
	std::function<void(void)> _on_disconnected;

	// запуск таймера переподключения
	void start_reconnection_timer();

	//приостановить таймер переподключений
	void pause_reconnection_timer();

	// остановка по внутренним причинам
	e_socket_result inner_close_connection();

	// вызывается при завершении работы потока обработки сокета
	void on_complete_stream_fn();

	CAsyncClientSocket(const CAsyncClientSocket& ob);

public:
	CAsyncClientSocket(tools::lock_vector<data_wrappers::_tag_data_const>& received_data,
					   std::function<void(tools::data_wrappers::_tag_data_managed)> on_data_received,
					   std::function<void(void)> on_connected,
					   std::function<void(void)> on_disconnected);
	virtual ~CAsyncClientSocket();

	// Постановка в очередь данных на отправку
	void PushBackToSend(data_wrappers::_tag_data_const data);

	// Постановка в начало очереди на отправку
	void PushFrontToSend(data_wrappers::_tag_data_const data);

	// открыть соединение с указанными параметрами
	e_socket_result OpenConnection(const tag_connection_params& params);

	// закрытие существующего подключения
	e_socket_result CloseConnection();

};
}
}


