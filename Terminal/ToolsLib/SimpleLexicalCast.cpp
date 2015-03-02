#include "stdafx.h"
#include "SimpleLexicalCast.h"

namespace tools
{
CSimpleLexicalCast::CSimpleLexicalCast(void)
	: _sstr_inp(std::wstring())
	, _sstr_outp(std::wstring())
	, _text_pattern(_T(""))
	, _locale("Russian")
{
	ZeroMemory(&_tm_struct, sizeof(_tm_struct));
	
}

CSimpleLexicalCast::~CSimpleLexicalCast(void)
{
}

BOOL CSimpleLexicalCast::set_text_pattern( const std::wstring& str )
{
	_text_pattern = str;
	return !_text_pattern.empty();
}

BOOL CSimpleLexicalCast::parse_date_time_string( const std::wstring& str )
{
	if (TRUE == str.empty())
		return FALSE;

	_sstr_inp.str(str);

	std::use_facet<std::time_get<TCHAR> >(_locale).get(_sstr_inp.rdbuf(),
															0,
															_sstr_inp,
															_io_state,
															&_tm_struct,
															_text_pattern.c_str(),
															_text_pattern.c_str() + _text_pattern.length());
	return (std::ios_base::failbit != _io_state);
}

void CSimpleLexicalCast::set_datetime_struct( const tm& tm_struct )
{
	_tm_struct = tm_struct;
}

std::wstring CSimpleLexicalCast::format_date_time()
{
	if (TRUE == _text_pattern.empty())
		return _T("");

	_sstr_outp.str(std::wstring());

	std::use_facet<std::time_put<TCHAR> >(_locale).put(_sstr_outp.rdbuf(),
													_sstr_outp,
													_T(' '),
													&_tm_struct,
													_text_pattern.c_str(),
													_text_pattern.c_str() + _text_pattern.length());
	return _sstr_outp.str();
}

LONG CSimpleLexicalCast::parse_long( const std::wstring& str )
{
	LONG result_val = 0;

	_sstr_inp.str(str);

	std::use_facet<std::num_get<TCHAR> > (_locale).get(_sstr_inp.rdbuf(),
														0,
														_sstr_inp,
														_io_state,
														result_val);

	if (std::ios_base::failbit == _io_state)
	{
		TRACE("fail in parse_long");
	}

	return result_val;
}

std::wstring CSimpleLexicalCast::long_to_str( const LONG& val )
{
	_sstr_outp.str(std::wstring());
	std::use_facet<std::num_put<TCHAR> > (_locale).put(_sstr_outp.rdbuf(),
														_sstr_outp,
														_T(' '),
														val);
	return _sstr_outp.rdbuf()->str();
}
}


