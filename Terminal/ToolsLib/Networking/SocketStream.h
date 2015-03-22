#pragma once
#include "stdafx/stdafx.h"
#include "lock_vector.h"
#include "raw_data_warappers.h"
#include "TraceError.h"
#include "SocketUtils.h"
#include <mutex>
#include <condition_variable>
#include "tools_structures.h"
#include "lock_deque.h"


/*!
 * \file SocketStream.h
 *
 * \author Victor Mulika
 * \date 06 Март 2015
 *
 * 
 */

namespace tools
{
namespace networking
{
// Класс сокета, производящий чтение и запись в сокет
// Работает в отдельном потоке
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
		ready,		// сокет готов для действия
		not_ready	// сокет не готов для действия
	};

	tools::logging::CTraceError* _tr_error;

	// работа с текстами ошибок
	std::wstringstream _str_str;

	SOCKET	_socket;

	// буфер для приёма данных
	CHAR _received_buffer[0x10000];

	// количество принятых байт
	INT _received_bytes_count;

	// принятые данные
	tools::lock_vector<data_wrappers::_tag_data_managed>& _received_data;

	// данные для отправки
	std::shared_ptr<tools::lock_deque<data_wrappers::_tag_data_const>> _data_to_send;

	// поток обработки данных
	std::thread _this_thread;

	// статус обработки данных
	e_work_loop_status _work_loop_status;

	// сюда пишем статус окончания работы
	e_work_loop_status* _end_status;

	tools::e_init_state _start_state;

	// вызывается по завершении потока
	std::function<void()> _on_complete_fn;

	// вызывается при получении данных
	std::function<void(tools::data_wrappers::_tag_data_managed)> _on_data_received;

	bool _thread_running;

	void thread_method();

	// очистка сокета
	void cleanup();

	// очистить буферы даннфх
	void clear_buffers();

	// проверка сокета 
	_e_check_socket_result check_socket(const _e_check_socket_type& cst);

	// приём данных
	e_socket_result receive_data();

	// отправка данных
	e_socket_result send_data();

	CSocketStream(const CSocketStream&);

public:

	CSocketStream(tools::lock_vector<data_wrappers::_tag_data_managed>& received_data,
				  std::function<void(tools::data_wrappers::_tag_data_managed)> on_data_received);
	virtual ~CSocketStream();

	// запуск
	e_socket_result Start(const SOCKET& socket_to_process,
						  e_work_loop_status* end_status,
						  std::function<void()> on_complete_fn);

	// остановка
	e_socket_result Stop();

	// Постановка в очередь данных на отправку
	void PushBackToSend(data_wrappers::_tag_data_const data);

	// Постановка в начало очереди на отправку
	void PushFrontToSend(data_wrappers::_tag_data_const data);

};
}
}



