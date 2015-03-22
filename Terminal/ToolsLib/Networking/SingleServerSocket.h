#pragma once
#include "TraceError.h"
#include "SocketUtils.h"
#include "raw_data_warappers.h"
#include "lock_vector.h"
#include "SocketStream.h"


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
	tools::logging::CTraceError* _tr_error;

	// работа с текстами ошибок
	std::wstringstream _str_str;

	SOCKET	_listen_socket;

	addrinfo	_addr_hints;
	addrinfo	*_addr_results;
	addrinfo	*_addrptr;

	// результат выполнени€ функции windows socket
	INT _socket_fn_result;

	tag_connection_params _connection_params;

	tools::e_init_state	_init_state;

	CSocketStream _socket_stream;

	e_work_loop_status _stream_end_status;

	std::mutex _wait_stream_mutex;
	std::unique_lock<std::mutex>* _lock_wait_stream;
	std::condition_variable _cv;

	// вызываетс€ при завершении работы потока обработки сокета
	void on_complete_stream_fn();

	// поток слушающего порта данных
	std::thread _listen_thread;

	// статус обработки данных
	e_work_loop_status _work_loop_status;

	// метод потока слушающего порта
	void listen_method();

	e_socket_result init();

	void cleanup();

	// true если _socket_fn_result равен valid_val
	bool check_socket_fn_result_and(const INT& valid_val);

	// true если _socket_fn_result не равен invalid_val
	bool check_socket_fn_result_not(const INT& invalid_val);

	CSingleServerSocket(const CSingleServerSocket& ob);

public:
	CSingleServerSocket(tools::lock_vector<data_wrappers::_tag_data_const>& received_data,
						std::function<void(tools::data_wrappers::_tag_data_managed)> on_data_received);
	virtual ~CSingleServerSocket();

	// ѕостановка в очередь данных на отправку
	void PushBackToSend(data_wrappers::_tag_data_const data);

	// ѕостановка в начало очереди на отправку
	void PushFrontToSend(data_wrappers::_tag_data_const data);

	// открыть соединение с указанными параметрами
	e_socket_result Start(const tag_connection_params& params);

	// закрытие существующего подключени€
	e_socket_result Stop();
};

}
}