#pragma once

#include "stdafx.h"

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

};