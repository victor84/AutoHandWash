#include "stdafx.h"
#include "TraceError.h"

namespace tools
{

namespace logging
{
CTraceError* CTraceError::_instance = NULL;

CTraceError::CTraceError(VOID)
	: _short_messages_storage(nullptr)
	, _full_messages_storage(nullptr)
{
	fill_type_mapping();
}

void CTraceError::fill_type_mapping()
{
	_v_type_text.clear();

	_v_type_text.push_back(make_type_mapping(TE_ERROR, MB_ICONSTOP, _T("Ошибка")));
	_v_type_text.push_back(make_type_mapping(TE_MESSAGE, MB_ICONINFORMATION, _T("Сообщение")));

}

CTraceError::_tag_msg_type_mapping CTraceError::make_type_mapping( TRACE_MSG_TYPE type, UINT msg_box_icon, std::wstring text )
{
	_tag_msg_type_mapping instance;

	instance.type = type;
	instance.msg_box_icon = msg_box_icon;
	instance.text = text;

	return instance;
}


CTraceError::~CTraceError()
{
	_instance = nullptr;
}

void CTraceError::trace( std::wstring text, std::string function_name, TRACE_MSG_TYPE type, BOOL show_msgbox /*= FALSE*/, CWnd* pParentWnd /*= NULL*/, DWORD thread_id /*= GetCurrentThreadId()*/ )
{
	std::wstringstream mes;

	mes << get_type_text(type);

	if (NULL != thread_id)
		mes << _T(" поток [") << thread_id << _T("] ");

	mes <<  _T("[") << text << _T("]");

	if (nullptr != _short_messages_storage)
		_short_messages_storage->push_back(text);

	if(!function_name.empty())
		mes << _T(" в функции [") << std::wstring(function_name.begin(), function_name.end()) << _T("]");
	mes << "\n";

	if (nullptr != _full_messages_storage)
		_full_messages_storage->push_back(mes.str());

	::OutputDebugString(mes.str().c_str());

	if(show_msgbox && pParentWnd)
	{
		MessageBox(pParentWnd->m_hWnd, text.c_str(), get_type_text(type).c_str(), MB_OK | get_type_msg_box_icon(type));
	}
}

void CTraceError::trace_com( HRESULT result, std::string function, TRACE_MSG_TYPE type, BOOL show_msgbox, CWnd* pParentWnd, DWORD thread_id )
{
	trace(_com_error(result).ErrorMessage(), function, type, show_msgbox, pParentWnd, thread_id);
}

CTraceError* CTraceError::get_instance()
{
	if(NULL == _instance)
		_instance = new CTraceError();
	return _instance;
}

void CTraceError::print( std::wstring text )
{
	OutputDebugString(text.c_str());
}

std::wstring CTraceError::format_sys_message( DWORD mes_id )
{
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					mes_id,
					NULL,
					message_buf,
					_countof(message_buf) - 1,
					NULL);

	PTCHAR p_endl = wcschr(message_buf, _T('\r'));
	if (NULL != p_endl)
		*p_endl = _T('\0');

	return message_buf;
}

void CTraceError::set_messages_storages(tools::lock_vector<std::wstring>* short_messages_storage, tools::lock_vector<std::wstring>* full_messages_storage)
{
	_short_messages_storage = short_messages_storage;
	_full_messages_storage = full_messages_storage;
}

std::wstring CTraceError::get_type_text(TRACE_MSG_TYPE type)
{
	_tag_msg_type_mapping* ptt = find_type_text(type);

	if (NULL == ptt)
		return _T("");

	return ptt->text;
}

CTraceError::_tag_msg_type_mapping* CTraceError::find_type_text( TRACE_MSG_TYPE type )
{
	std::vector< _tag_msg_type_mapping >::iterator t_iter = _v_type_text.begin();

	for (; _v_type_text.end() != t_iter; ++t_iter )
	{
		if (type == t_iter->type)
			break;
	}

	if (_v_type_text.end() == t_iter)
	{
		OutputDebugString(_T("тип сообщения не найден"));
		return NULL;
	}
	return &(*t_iter);
}

UINT CTraceError::get_type_msg_box_icon( TRACE_MSG_TYPE type )
{
	_tag_msg_type_mapping* ptt = find_type_text(type);

	if (NULL == ptt)
		return NULL;

	return ptt->msg_box_icon;
}
}

}

