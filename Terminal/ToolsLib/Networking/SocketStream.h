#pragma once
#include "stdafx/stdafx.h"
#include "lock_vector.h"
#include "raw_data_warappers.h"
#include "TraceError.h"
#include "SocketUtils.h"
#include <mutex>
#include <condition_variable>


/*!
 * \file SocketStream.h
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
//  ласс сокета, производ€щий чтение и запись в сокет
// –аботает в отдельном потоке
class CSocketStream
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

	SOCKET	_client_socket;

	// буфер дл€ приЄма данных
	CHAR _received_buffer[0x10000];

	// количество прин€тых байт
	INT _received_bytes_count;

	// прин€тые данные
	tools::lock_vector<data_wrappers::_tag_data_const>& _received_data;

	// данные дл€ отправки
	tools::lock_vector<data_wrappers::_tag_data_const> _data_to_send;

	// поток обработки данных
	std::thread _this_thread;

	// статус обработки данных
	_e_work_loop_status _work_loop_status;

	// сюда пишем статус окончани€ работы
	_e_work_loop_status* _end_status;

	_e_init_state _start_state;

	std::function<void()> _on_complete_fn;

	void thread_method();

	// очистка сокета
	void cleanup();

	// проверка сокета 
	_e_check_socket_result check_socket(const _e_check_socket_type& cst);

	// приЄм данных
	e_socket_result receive_data();

	// отправка данных
	e_socket_result send_data();

	CSocketStream(const CSocketStream&);

public:

	CSocketStream(tools::lock_vector<data_wrappers::_tag_data_const>& received_data);
	virtual ~CSocketStream();

	// запуск
	e_socket_result Start(const SOCKET& socket_to_process,
						  _e_work_loop_status* end_status,
						  std::function<void()> on_complete_fn);

	// остановка
	e_socket_result Stop();

	void Send(data_wrappers::_tag_data_const data);

};
}
}



