#pragma once

#include "stdafx.h"
#include "raw_data_warappers.h"

///////////////////////////////////////////
// ��������������� �������
///////////////////////////////////////////

namespace tools
{
	// �������� �� ��������� ������ �������, ��������� � �������� ���������
	void remove_extra_backslash(CString& str);
	void remove_extra_backslash(std::wstring& str);

	// ������� � ������ ������� ��������� ���� ��� �������� ����
	void insert_double_backslash(std::wstring& str);

	// ������� �����
	void clear_folder( CString szPath );

	// �������� �������� ��� ����� ��� ����� (���� � ������ ��� ���������� ����� �� ��������� ��������� ��������� ������)
	BOOL create_sub_folders(CString szFilePath);

	// ��������� ������ ���������� �����
	// TRUE ���� � ���������� ����� ���� ������
	BOOL get_file_version(std::wstring file_name, std::wstring& sz_version, std::wstring* sz_lang = nullptr);

	// ���������� ���� ����� ��� ����� �� �����
	// FALSE ���� �� ������� ��������� ����
	BOOL split_path(IN const std::wstring& path, OUT std::wstring* p_drive = nullptr, OUT std::wstring* p_dir = nullptr, OUT std::wstring* p_name = nullptr, OUT std::wstring* p_ext = nullptr, OUT std::wstring* p_name_and_ext = nullptr);

	// ��������� ������� ����� �������� ������������ �����
	// c ��������� � �����
	BOOL get_module_directory(OUT std::wstring& path);
	BOOL get_module_directory(OUT CString& path);

	// ���������� � ���������� ������� �������� �������� �����
	// FALSE ��� �������
	BOOL add_time_bias(COleDateTime& date_time);

	// ��������������� �����
	std::wstring string_to_wstring(const std::string& str);
	std::string wstring_to_string(const std::wstring& str);

	// ����� �������� ������ � ������� hex
	std::wstring binary_to_hex(const data_wrappers::_tag_data_const& bin_data);

	// hex ������ � �������� ������
	data_wrappers::_tag_data_managed hex_to_binary(const std::wstring& hex_string);

	// �������� �� ��������� �� �������
	template<typename _ColType, typename _PredicateType>
	_ColType remove_if(_ColType begin, _ColType end, _PredicateType predicate)
	{
		_ColType dest = begin;
		for (_ColType itr = begin; itr != end; ++itr)
		{
			if (!predicate(*itr))
				*(dest++) = *itr;
		}
		return dest;
	}

	uint32_t char_to_int(char input);

};