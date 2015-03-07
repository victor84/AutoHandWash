#pragma once
#include "lock_vector.h"

namespace tools
{

namespace logging
{
/*******************************************************
CTraceError
класс для отображения и логирования ошибок и сообщений
сообщения и ошибки пишет в лог

для получения указателя на экземпляр
использовать get_instance
использовать delete для полученного экземпляра один раз
*******************************************************/
class CTraceError
{
public:
	// тип отображаемх сообщений
	enum TRACE_MSG_TYPE
	{
		TE_ERROR = 0,	// выводится как ошибка
		TE_MESSAGE	// выводится как сообщение
	};

private:
	static CTraceError* _instance;

	TCHAR message_buf[1024];

	// соответствие типа сообщения
	struct _tag_msg_type_mapping 
	{
		TRACE_MSG_TYPE type;
		UINT msg_box_icon;
		std::wstring text;
	};

	// соответствия
	std::vector <_tag_msg_type_mapping> _v_type_text;

	// хранилище сообщений лога без дополнительной информации
	tools::lock_vector<std::wstring>* _short_messages_storage;

	tools::lock_vector<std::wstring>* _full_messages_storage;

	void fill_type_mapping();

	_tag_msg_type_mapping make_type_mapping(TRACE_MSG_TYPE type, UINT msg_box_icon, std::wstring text);

	_tag_msg_type_mapping* find_type_text(TRACE_MSG_TYPE type);

	UINT get_type_msg_box_icon(TRACE_MSG_TYPE type);

	// возвращает текст типа сообщения
	std::wstring get_type_text(TRACE_MSG_TYPE type);

protected:
	CTraceError(void);

public:
	virtual ~CTraceError();

	//************************************
	// Метод:      get_instance
	// Полное имя: CTraceError::get_instance
	// Доступ:     public 
	// Возвращает: указатель на экземпляр класса
	// Описание:   использовать для получения экземпляра для дальнейшео использования
	//************************************
	static CTraceError* get_instance();

	//************************************
	// Метод:      trace
	// Полное имя: CTraceError::trace
	// Доступ:     public 
	// Описание:   отображает сообщения в debug и MessageBox
	// Параметр:   text - текст сообщения
	// Параметр:   function - имя функции
	// Параметр:   type - тип сообщения
	// Параметр:   show_msgbox - если TRUE, вывести MessageBox с текстом
	// Параметр:   pParentWnd - окно - владелец выводимого MessageBox
	//************************************
	void trace(std::wstring text, std::string function_name, TRACE_MSG_TYPE type, BOOL show_msgbox = FALSE, CWnd* pParentWnd = NULL, DWORD thread_id = GetCurrentThreadId());

	//****************************************************
	// Метод:      trace_com
	// Полное имя: CTraceError::trace_com
	// Доступ:     public 
	// Возвращает: void
	// Описание:   отображает сообщения или ошибки, возвращённые com функциями
	// Параметр:   result - результат выполнения функции
	// Параметр:   function - имя функции
	// Параметр:   type - тип сообщения
	// Параметр:   show_msgbox - если TRUE, вывести MessageBox с текстом
	// Параметр:   pParentWnd - окно - владелец выводимого MessageBox
	//***************************************************
	void trace_com(HRESULT result, std::string function, TRACE_MSG_TYPE type, BOOL show_msgbox, CWnd* pParentWnd, DWORD thread_id);

	// макросы для быстрого доступа к методу trace
	// с подстановленными параметрами
	#define trace_error(_text)			trace(_text, __FUNCSIG__, tools::logging::CTraceError::TE_ERROR, FALSE, NULL, GetCurrentThreadId())
	#define trace_com_error(_result)	trace_com(_result, __FUNCSIG__, tools::logging::CTraceError::TE_ERROR, FALSE, NULL, GetCurrentThreadId())
	#define trace_message(_text)		trace(_text, __FUNCSIG__, tools::logging::CTraceError::TE_MESSAGE, FALSE, NULL, GetCurrentThreadId())
	#define msg_box_error(_text)		trace(_text, __FUNCSIG__, tools::logging::CTraceError::TE_ERROR, TRUE, this, GetCurrentThreadId())
	#define msg_box_msg(_text)			trace(_text, __FUNCSIG__, tools::logging::CTraceError::TE_MESSAGE, TRUE, this, GetCurrentThreadId())

	//****************************************************
	// Метод:      print
	// Полное имя: CTraceError::print
	// Доступ:     public 
	// Описание:   отображает текст в debug
	// Параметр:   std::string text
	//***************************************************
	void print(std::wstring text);

	// возвращает текст системной ошибки по её коду
	std::wstring format_sys_message(DWORD mes_id);

	// задание хранилищ для сообщений
	void set_messages_storages(tools::lock_vector<std::wstring>* short_messages_storage,
							   tools::lock_vector<std::wstring>* full_messages_storage);

};
}

}


