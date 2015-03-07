#pragma once
#include "lock_vector.h"

namespace tools
{

namespace logging
{
/*******************************************************
CTraceError
����� ��� ����������� � ����������� ������ � ���������
��������� � ������ ����� � ���

��� ��������� ��������� �� ���������
������������ get_instance
������������ delete ��� ����������� ���������� ���� ���
*******************************************************/
class CTraceError
{
public:
	// ��� ����������� ���������
	enum TRACE_MSG_TYPE
	{
		TE_ERROR = 0,	// ��������� ��� ������
		TE_MESSAGE	// ��������� ��� ���������
	};

private:
	static CTraceError* _instance;

	TCHAR message_buf[1024];

	// ������������ ���� ���������
	struct _tag_msg_type_mapping 
	{
		TRACE_MSG_TYPE type;
		UINT msg_box_icon;
		std::wstring text;
	};

	// ������������
	std::vector <_tag_msg_type_mapping> _v_type_text;

	// ��������� ��������� ���� ��� �������������� ����������
	tools::lock_vector<std::wstring>* _short_messages_storage;

	tools::lock_vector<std::wstring>* _full_messages_storage;

	void fill_type_mapping();

	_tag_msg_type_mapping make_type_mapping(TRACE_MSG_TYPE type, UINT msg_box_icon, std::wstring text);

	_tag_msg_type_mapping* find_type_text(TRACE_MSG_TYPE type);

	UINT get_type_msg_box_icon(TRACE_MSG_TYPE type);

	// ���������� ����� ���� ���������
	std::wstring get_type_text(TRACE_MSG_TYPE type);

protected:
	CTraceError(void);

public:
	virtual ~CTraceError();

	//************************************
	// �����:      get_instance
	// ������ ���: CTraceError::get_instance
	// ������:     public 
	// ����������: ��������� �� ��������� ������
	// ��������:   ������������ ��� ��������� ���������� ��� ���������� �������������
	//************************************
	static CTraceError* get_instance();

	//************************************
	// �����:      trace
	// ������ ���: CTraceError::trace
	// ������:     public 
	// ��������:   ���������� ��������� � debug � MessageBox
	// ��������:   text - ����� ���������
	// ��������:   function - ��� �������
	// ��������:   type - ��� ���������
	// ��������:   show_msgbox - ���� TRUE, ������� MessageBox � �������
	// ��������:   pParentWnd - ���� - �������� ���������� MessageBox
	//************************************
	void trace(std::wstring text, std::string function_name, TRACE_MSG_TYPE type, BOOL show_msgbox = FALSE, CWnd* pParentWnd = NULL, DWORD thread_id = GetCurrentThreadId());

	//****************************************************
	// �����:      trace_com
	// ������ ���: CTraceError::trace_com
	// ������:     public 
	// ����������: void
	// ��������:   ���������� ��������� ��� ������, ������������ com ���������
	// ��������:   result - ��������� ���������� �������
	// ��������:   function - ��� �������
	// ��������:   type - ��� ���������
	// ��������:   show_msgbox - ���� TRUE, ������� MessageBox � �������
	// ��������:   pParentWnd - ���� - �������� ���������� MessageBox
	//***************************************************
	void trace_com(HRESULT result, std::string function, TRACE_MSG_TYPE type, BOOL show_msgbox, CWnd* pParentWnd, DWORD thread_id);

	// ������� ��� �������� ������� � ������ trace
	// � ���������������� �����������
	#define trace_error(_text)			trace(_text, __FUNCSIG__, tools::logging::CTraceError::TE_ERROR, FALSE, NULL, GetCurrentThreadId())
	#define trace_com_error(_result)	trace_com(_result, __FUNCSIG__, tools::logging::CTraceError::TE_ERROR, FALSE, NULL, GetCurrentThreadId())
	#define trace_message(_text)		trace(_text, __FUNCSIG__, tools::logging::CTraceError::TE_MESSAGE, FALSE, NULL, GetCurrentThreadId())
	#define msg_box_error(_text)		trace(_text, __FUNCSIG__, tools::logging::CTraceError::TE_ERROR, TRUE, this, GetCurrentThreadId())
	#define msg_box_msg(_text)			trace(_text, __FUNCSIG__, tools::logging::CTraceError::TE_MESSAGE, TRUE, this, GetCurrentThreadId())

	//****************************************************
	// �����:      print
	// ������ ���: CTraceError::print
	// ������:     public 
	// ��������:   ���������� ����� � debug
	// ��������:   std::string text
	//***************************************************
	void print(std::wstring text);

	// ���������� ����� ��������� ������ �� � ����
	std::wstring format_sys_message(DWORD mes_id);

	// ������� �������� ��� ���������
	void set_messages_storages(tools::lock_vector<std::wstring>* short_messages_storage,
							   tools::lock_vector<std::wstring>* full_messages_storage);

};
}

}


