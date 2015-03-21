#include "stdafx.h"
#include "SettingsLoader.h"
#include "CriticalSectionLocker.h"
#include "tools.h"

namespace tools
{
namespace settings
{
using namespace tools::threading;

CSettingsLoader* CSettingsLoader::_instance = nullptr;

CSettingsLoader::CSettingsLoader(void)
{
	_tr_error = tools::logging::CTraceError::get_instance();
	ZeroMemory(_buf, sizeof(_buf));

	create_file_name();
}

CSettingsLoader* CSettingsLoader::get_instance()
{
	if (nullptr == _instance)
	{
		_instance = new CSettingsLoader();
	}
	return _instance;
}

CSettingsLoader::~CSettingsLoader(void)
{
}

void CSettingsLoader::add_parameter(const e_parameter_data_type& type, 
									const CString& group, 
									const CString name)
{
	if (TRUE == find_parameter(name, group))
		return;

	_v_parameters.push_back(create_param(type, group, name));
}

CSettingsLoader::_tag_parameter CSettingsLoader::create_param(const e_parameter_data_type& type, 
															  const CString& group, 
															  const CString name)
{
	_tag_parameter param(_lex_cast);

	param.group = group;
	param.name = name;
	param.val.data_type = type;
	return param;
}

BOOL CSettingsLoader::find_parameter(const CString& param_name, const CString& group_name)
{
	_current_parameter = _v_parameters.begin();
	for (;_v_parameters.end() != _current_parameter; ++_current_parameter)
	{
		if ((param_name == _current_parameter->name)&&
			(group_name == _current_parameter->group))
			return TRUE;
	}
	return FALSE;
}

BOOL CSettingsLoader::read_current_parameter()
{
	if (_v_parameters.end() == _current_parameter)
		return FALSE;

	_current_parameter->val.clear();

	ZeroMemory(_buf, sizeof(_buf));
	INT read_count = ::GetPrivateProfileString(_current_parameter->group,
											   _current_parameter->name,
											   CString(_current_parameter->val),
											   _buf,
											   _countof(_buf),
											   _file_name);

	if (0 == read_count)
		return TRUE;

	switch (_current_parameter->val.data_type)
	{
	case e_parameter_data_type::type_string		: _current_parameter->val = _buf;						break;
	case e_parameter_data_type::type_int		: _current_parameter->val = _lex_cast.parse_long(_buf);	break;
	}
	return TRUE;
}

BOOL CSettingsLoader::write_current_parameter()
{
	return ::WritePrivateProfileString(_current_parameter->group,
											_current_parameter->name,
											CStringW(_current_parameter->val),
											_file_name);
}

BOOL CSettingsLoader::read_all()
{
	CCriticalSectionLocker cs_lock(_cs);

	_current_parameter = _v_parameters.begin();
	std::wstring error_str = _T("");
	BOOL result = TRUE;

	for (; _v_parameters.end() != _current_parameter; ++_current_parameter)
	{
		if (FALSE == read_current_parameter())
		{
			error_str = _T("не удалось прочитать параметр ") + _current_parameter->name;
			_tr_error->trace_error(error_str);
			result = FALSE;
		}
	}

	return result;
}

BOOL CSettingsLoader::read(const CString& parameter_name, const CString& group_name)
{
	CCriticalSectionLocker cs_lock(_cs);
	if (FALSE == find_parameter(parameter_name, group_name))
		return FALSE;

	return read_current_parameter();
}

BOOL CSettingsLoader::create_file_name()
{
	if (FALSE == tools::get_module_directory(_file_name))
		return FALSE;

	_file_name += "settings.ini";

	return TRUE;
}

CStringW CSettingsLoader::get_string(const CString& parameter_name, const CString& group_name)
{
	CCriticalSectionLocker cs_lock(_cs);
	if (FALSE == find_parameter(parameter_name, group_name))
		return L"";

	return *_current_parameter;
}

INT CSettingsLoader::get_int(const CString& parameter_name, const CString& group_name)
{
	CCriticalSectionLocker cs_lock(_cs);
	if (FALSE == find_parameter(parameter_name, group_name))
		return 0;
	
	return *_current_parameter;
}

BOOL CSettingsLoader::save_parameter(const CString& parameter_name, const CString& group_name, const CString& str)
{
	CCriticalSectionLocker cs_lock(_cs);
	if (FALSE == find_parameter(parameter_name, group_name))
		return FALSE;

	_current_parameter->val = str;
	return write_current_parameter();
}

BOOL CSettingsLoader::save_parameter(const CString& parameter_name, const CString& group_name, const INT& val)
{
	CCriticalSectionLocker cs_lock(_cs);
	if (FALSE == find_parameter(parameter_name, group_name))
		return FALSE;

	_current_parameter->val = val;
	return write_current_parameter();
}

}
}

