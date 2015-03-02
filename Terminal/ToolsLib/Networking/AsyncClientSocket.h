#pragma once
#include "TraceError.h"
#include "SocketUtils.h"
#include "raw_data_warappers.h"
#include "lock_vector.h"

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
		not_init,
		was_init
	};

	// тип проверки сокета на готовность
	enum class _e_check_socket_type
	{
		read,	// чтение
		write	// запись
	};

	// результат проверки сокета
	enum class _e_check_socket_result
	{
		error,		// ошибка при проверке
		ready,		// сокет готов для действия
		not_ready	// сокет не готов для действия
	};


	tools::logging::CTraceError* _tr_error;

	// работа с текстами ошибок
	std::wstringstream _str_str;

	SOCKET		_client_socket;

	addrinfo	_addr_hints;
	addrinfo	*_addr_results;
	addrinfo	*_addrptr;

	// буфер для приёма данных
	CHAR _received_buffer[0x10000];

	// количество принятых байт
	INT _received_bytes_count;

	// результат выполнения функции windows socket
	INT _socket_fn_result;

	tag_connection_params _connection_params;

	_e_connection_state _connection_state;
	_e_init_state _init_state;

	// принятые данные
	tools::lock_vector<data_wrappers::_tag_data_const>& _received_data;

	// данные для отправки
	tools::lock_vector<data_wrappers::_tag_data_const> _data_to_send;

	// поток обработки данных
	std::thread _this_thread;

	// статус обработки данных
	_e_work_loop_status _work_loop_status;

	void thread_method();

	e_socket_result init();

	void cleanup();

	// true если _socket_fn_result равен valid_val
	bool check_socket_fn_result_and(const INT& valid_val);

	// true если _socket_fn_result не равен invalid_val
	bool check_socket_fn_result_not(const INT& invalid_val);

	// проверка сокета 
	_e_check_socket_result check_socket(const _e_check_socket_type& cst);

	// приём данных
	e_socket_result receive_data();

	// отправка данных
	e_socket_result send_data();

	// таймер переподключения
	Concurrency::timer<INT>* _reconnection_timer;

	// метод переподключения
	Concurrency::call<INT> _reconnection_method;

	// запуск таймера переподключения
	void start_reconnection_timer();

	//приостановить таймер переподключений
	void pause_reconnection_timer();

	// остановка по внутренним причинам
	e_socket_result inner_close_connection();

	CAsyncClientSocket(const CAsyncClientSocket& ob);

public:
	CAsyncClientSocket(tools::lock_vector<data_wrappers::_tag_data_const>& received_data);
	virtual ~CAsyncClientSocket();


	void Send(data_wrappers::_tag_data_const data);

	// открыть соединение с указанными параметрами
	e_socket_result OpenConnection(const tag_connection_params& params);

	// закрытие существующего подключения
	e_socket_result CloseConnection();

};
}
}


