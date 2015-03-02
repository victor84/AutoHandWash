#pragma once

namespace tools
{

class CSimpleLexicalCast
{
	// �����
	struct tm _tm_struct;

	// ������
	std::locale _locale;

	// �����, �� �������� ����������� ������ ���� � �������
	std::wstringstream _sstr_inp;

	// �����, � ������� ������� ��������������� ���� � �����
	std::wstringstream _sstr_outp;

	// ��������� ��������������
	std::ios_base::iostate _io_state;

	// ������ ��� ��������������
	std::wstring _text_pattern;

public:
	CSimpleLexicalCast(void);
	virtual ~CSimpleLexicalCast(void);

	tm get_datetime_struct() const { return _tm_struct; }
	time_t get_time_t() { return mktime(&_tm_struct); }

	void set_datetime_struct(const tm& tm_struct);

	// ��������� ���������� �������
	BOOL set_text_pattern(const std::wstring& str);

	// ����������� ������ ������ ����
	BOOL parse_date_time_string(const std::wstring& str);

	// �������������� ���� � �������
	std::wstring format_date_time();

	LONG parse_long(const std::wstring& str);

	std::wstring long_to_str(const LONG& val);
};
}


