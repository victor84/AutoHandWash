#pragma once

#include "stdafx.h"
#include "raw_data_warappers.h"

///////////////////////////////////////////
// вспомогательные функции
///////////////////////////////////////////

namespace tools
{
	// удаление из указанной строки двойных, начальных и конечных бэкслэшей
	void remove_extra_backslash(CString& str);
	void remove_extra_backslash(std::wstring& str);

	// вставка в строку двойных бэкслэшей туда где вставлен один
	void insert_double_backslash(std::wstring& str);

	// очистка папки
	void clear_folder( CString szPath );

	// создание подпапок для файла или папки (если у строки нет расширения слово за последним бэкслэшем считается папкой)
	BOOL create_sub_folders(CString szFilePath);

	// получнеие версии указанного файла
	// TRUE если у указанного файла есть версия
	BOOL get_file_version(std::wstring file_name, std::wstring& sz_version, std::wstring* sz_lang = nullptr);

	// разделение пути файла или папки на части
	// FALSE если не удалось разделить путь
	BOOL split_path(IN const std::wstring& path, OUT std::wstring* p_drive = nullptr, OUT std::wstring* p_dir = nullptr, OUT std::wstring* p_name = nullptr, OUT std::wstring* p_ext = nullptr, OUT std::wstring* p_name_and_ext = nullptr);

	// получение полного имени каталога исполняемого файла
	// c бэкслэшем в конце
	BOOL get_module_directory(OUT std::wstring& path);
	BOOL get_module_directory(OUT CString& path);

	// прибавляет к указанному времени величину часового пояса
	// FALSE при неудаче
	BOOL add_time_bias(COleDateTime& date_time);

	// конвертирование строк
	std::wstring string_to_wstring(const std::string& str);
	std::string wstring_to_string(const std::wstring& str);

	// вывод бинарных данных в формате hex
	std::wstring binary_to_hex(const data_wrappers::_tag_data_const& bin_data);

	// hex строка в бинарные данные
	data_wrappers::_tag_data_managed hex_to_binary(const std::wstring& hex_string);

	// удаление из коллекции по условию
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