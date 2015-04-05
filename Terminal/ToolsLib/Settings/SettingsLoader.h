#pragma once
#include "stdafx\stdafx.h"
#include "TraceError.h"
#include "..\SimpleLexicalCast.h"

/************************************************************************
CSettingsLoader
 ласс дл€ чтени€ настроек приложени€.
ќдиночка.
************************************************************************/

namespace tools
{
namespace settings
{

class CSettingsLoader
{
public:

	// тип значени€ параметра
	enum class e_parameter_data_type
	{
		error	= -1,
		type_string	= 0,
		type_int
	};


	struct _tag_parameter_value
	{
		e_parameter_data_type		data_type;
		CString						str_val;
		INT							int_val;

		const CString				default_str_val;
		const INT					default_int_val;

		_tag_parameter_value()
			: data_type(e_parameter_data_type::error)
			, str_val(L"")
			, int_val(0)
			, default_str_val(L"")
			, default_int_val(0)
			, _lex_cast(nullptr)
		{

		}

		_tag_parameter_value(::tools::CSimpleLexicalCast* lex_cast)
			: data_type(e_parameter_data_type::error)
			, str_val(L"")
			, int_val(0)
			, default_str_val(L"")
			, default_int_val(0)
			, _lex_cast(lex_cast)
		{

		}

		_tag_parameter_value(const _tag_parameter_value& val)
			:default_str_val(L"")
			, default_int_val(0)
			, _lex_cast(val._lex_cast)
		{
			copy_ob(val);
		}

		operator INT()
		{
			switch (data_type)
			{
			case e_parameter_data_type::type_string		:	return _lex_cast->parse_long(CString(str_val).GetString());
			case e_parameter_data_type::type_int		:	return int_val;
			default										:	return default_int_val;
			}
		}

		operator CStringW()
		{
			switch (data_type)
			{
			case e_parameter_data_type::type_string		:	return CStringW(str_val);
			case e_parameter_data_type::type_int		:	return CStringW(_lex_cast->long_to_str(int_val).c_str());
			default										:	return CStringW(default_str_val);
			}
		}

		operator CStringA()
		{
			switch (data_type)
			{
			case e_parameter_data_type::type_string		:	return CStringA(str_val);
			case e_parameter_data_type::type_int		:	return CStringA(_lex_cast->long_to_str(int_val).c_str());
			default										:	return CStringA(default_str_val);
			}

		}

		_tag_parameter_value& operator=(const INT& val)
		{
			int_val = val;

			return *this;
		}

		_tag_parameter_value& operator=(const CStringW& val)
		{
			str_val = val;

			return *this;
		}

		_tag_parameter_value& operator=(const CStringA& val)
		{
			data_type = e_parameter_data_type::type_string;
			str_val = val;

			return *this;
		}

		_tag_parameter_value& operator=(const _tag_parameter_value& val)
		{
			copy_ob(val);
			return *this;
		}

		void clear()
		{
			str_val = default_str_val;
			int_val = default_int_val;
		}

	private:

		::tools::CSimpleLexicalCast* _lex_cast;

		void copy_ob(const _tag_parameter_value& val)
		{
			this->_lex_cast = val._lex_cast;
			this->data_type = val.data_type;
			this->str_val = val.str_val;
			this->int_val = val.int_val;
		}

	};

private:

	struct _tag_parameter
	{
		CString group;
		CString name;

		_tag_parameter_value val;

		_tag_parameter(CSimpleLexicalCast& lex_cast) 
			: group(_T(""))
			, name(_T(""))
			, val(&lex_cast)
		{

		}

		operator INT()
		{
			return val;
		}

		operator CStringW()
		{
			return val;
		}
	};

	
	tools::logging::CTraceError * _tr_error;
	static CSettingsLoader* _instance;

	CCriticalSection _cs;

	// буфер дл€ чтени€ параметров
	TCHAR _buf[0x400];

	// им€ ini файла
	CString _file_name;

	// преобразование
	CSimpleLexicalCast _lex_cast;

	// параметры
	std::vector<_tag_parameter> _v_parameters;

	// текущий параметр
	std::vector<_tag_parameter>::iterator _current_parameter;

	BOOL create_file_name();

	_tag_parameter create_param(const e_parameter_data_type& type, 
								const CString& group, 
								const CString name);

	BOOL find_parameter(const CString& param_name, const CString& group_name);

	BOOL read_current_parameter();

	BOOL write_current_parameter();

	CSettingsLoader(void);
public:

	static CSettingsLoader* get_instance();

	virtual ~CSettingsLoader(void);

	// ƒобавление параметра дл€ чтени€ из файла
	void add_parameter(const e_parameter_data_type& type,
					   const CString& group,
					   const CString name);

	// чтение всех настроек
	BOOL read_all();

	// чтение указанного параметра
	BOOL read(const CString& parameter_name, const CString& group_name);

	CStringW get_string(const CString& parameter_name, const CString& group_name);
	INT get_int(const CString& parameter_name, const CString& group_name);

	BOOL		save_parameter(const CString& parameter_name, const CString& group_name, const CString& str);
	BOOL		save_parameter(const CString& parameter_name, const CString& group_name, const INT& val);
};

}
}
