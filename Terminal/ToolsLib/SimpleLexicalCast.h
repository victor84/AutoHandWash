#pragma once

namespace tools
{

class CSimpleLexicalCast
{
	// время
	struct tm _tm_struct;

	// локаль
	std::locale _locale;

	// поток, из которого прозводится разбор даты и времени
	std::wstringstream _sstr_inp;

	// поток, в который пишется форматированная дата и время
	std::wstringstream _sstr_outp;

	// результат преобразования
	std::ios_base::iostate _io_state;

	// шаблон для преобразования
	std::wstring _text_pattern;

public:
	CSimpleLexicalCast(void);
	virtual ~CSimpleLexicalCast(void);

	tm get_datetime_struct() const { return _tm_struct; }
	time_t get_time_t() { return mktime(&_tm_struct); }

	void set_datetime_struct(const tm& tm_struct);

	// установка текстового шаблона
	BOOL set_text_pattern(const std::wstring& str);

	// лексический разбор строки даты
	BOOL parse_date_time_string(const std::wstring& str);

	// форматирование даты и времени
	std::wstring format_date_time();

	LONG parse_long(const std::wstring& str);

	std::wstring long_to_str(const LONG& val);
};
}


