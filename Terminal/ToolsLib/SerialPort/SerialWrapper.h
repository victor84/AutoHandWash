#pragma once
#include "SerialUtils.h"
#include "tools_structures.h"
#include "TraceError.h"
#include "raw_data_warappers.h"
#include "lock_deque.h"
#include "Serial.h"

namespace tools
{
namespace serial_port
{

class CSerialWrapper
{
	tools::logging::CTraceError* _tr_error;

	tag_connection_params _connection_params;

	// состо€ние инициализации
	tools::e_init_state _init_state;

	// прин€тые данные
	tools::lock_vector<data_wrappers::_tag_data_managed>& _received_data;

	// данные дл€ отправки
	std::shared_ptr<tools::lock_deque<data_wrappers::_tag_data_const>> _data_to_send;

	// поток обработки данных
	std::thread _this_thread;

	// статус обработки данных
	e_work_loop_status _work_loop_status;

	// флаг выполнени€ потока
	bool _thread_running;

	// COM порт
	CSerial _serial_port;

	// буфер чтени€ порта
	BYTE _read_buffer[1024];

	// буфер дл€ записи в порт
	BYTE _write_buffer[1024];

	// вызываетс€ при получении данных
	std::function<void(tools::data_wrappers::_tag_data_managed)> _on_data_received;

	void thread_method();

	// инициализаци€ порта
	e_serial_result init();

	// очистка порта
	void cleanup();

	// очистить буферы данных
	void clear_buffers();

	// получение данных с порта
	e_serial_result receive_data();

	// отправка данных
	e_serial_result send_data();

public:
	CSerialWrapper(tools::lock_vector<data_wrappers::_tag_data_managed>& received_data,
				   std::function<void(tools::data_wrappers::_tag_data_managed)> on_data_received);
	virtual ~CSerialWrapper();

	// открыть порт с указанными параметрами
	e_serial_result Start(const tag_connection_params& params);

	void Stop();

	// ѕостановка в очередь данных на отправку
	void PushBackToSend(data_wrappers::_tag_data_const data);

	// ѕостановка в начало очереди на отправку
	void PushFrontToSend(data_wrappers::_tag_data_const data);

};

}
}