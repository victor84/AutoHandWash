#pragma once
#include "TraceError.h"
#include "SocketUtils.h"
#include "raw_data_warappers.h"
#include "lock_vector.h"


/*!
 * \file SingleServerSocket.h
 *
 * \author Victor Mulika
 * \date 06 ћарт 2015
 *
 * 
 */



namespace tools
{
namespace networking
{
class CSingleServerSocket
{


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
		ready,		// сокет готов дл€ действи€
		not_ready	// сокет не готов дл€ действи€
	};

	tools::logging::CTraceError* _tr_error;

	// работа с текстами ошибок
	std::wstringstream _str_str;

	SOCKET	_listen_socket;
	SOCKET	_client_socket;

	addrinfo	_addr_hints;
	addrinfo	*_addr_results;
	addrinfo	*_addrptr;

	// буфер дл€ приЄма данных
	CHAR _received_buffer[0x10000];

	// количество прин€тых байт
	INT _received_bytes_count;

	// результат выполнени€ функции windows socket
	INT _socket_fn_result;

	tag_connection_params _connection_params;

	_e_init_state		_init_state;

	// прин€тые данные
	tools::lock_vector<data_wrappers::_tag_data_const>& _received_data;

	// данные дл€ отправки
	tools::lock_vector<data_wrappers::_tag_data_const> _data_to_send;

	// поток слушающего порта данных
	std::thread _listen_thread;

	// поток приЄма данных
	std::thread _client_thread;

	// статус обработки данных
	_e_work_loop_status _work_loop_status;

	// флаг, что можно принимать новые соединени€
	bool _can_new_accept;

	// метод потока слушающего порта
	void listen_method();

	// метод принимающего сокета
	void client_method();

	e_socket_result init();

	void cleanup();

	// true если _socket_fn_result равен valid_val
	bool check_socket_fn_result_and(const INT& valid_val);

	// true если _socket_fn_result не равен invalid_val
	bool check_socket_fn_result_not(const INT& invalid_val);

	// проверка сокета 
	_e_check_socket_result check_socket(const _e_check_socket_type& cst);

	// приЄм данных
	e_socket_result receive_data();

	// отправка данных
	e_socket_result send_data();

	CSingleServerSocket(const CSingleServerSocket& ob);

public:
	CSingleServerSocket(tools::lock_vector<data_wrappers::_tag_data_const>& received_data);
	virtual ~CSingleServerSocket();


	void Send(data_wrappers::_tag_data_const data);

	// открыть соединение с указанными параметрами
	e_socket_result Start(const tag_connection_params& params);

	// закрытие существующего подключени€
	e_socket_result Stop();
};

}
}