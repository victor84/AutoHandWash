#include "stdafx.h"
#include "tools.h"

void tools::remove_extra_backslash( CString& str )
{
	// удаляем двойные "\\"
	INT pos = -1;
	while (-1 != (pos = str.Find(_T("\\\\"))))
	{
		str.Delete(pos);
		pos = str.Find(_T("\\\\"));
	} while (-1 != pos);

	// удаляем начальные и конечные "\\"
	if ( _T('\\') == str.GetAt(0))
		str.Delete(0);
	if ((FALSE == str.IsEmpty()) && (_T('\\') == str.GetAt(str.GetLength() - 1)))
		str.Delete(str.GetLength() - 1);
}

void tools::remove_extra_backslash( std::wstring& str )
{
	CString cstr = str.c_str();
	remove_extra_backslash(cstr);
	str = cstr.GetString();
}

void tools::insert_double_backslash( std::wstring& str )
{
	if (TRUE == str.empty())
		return;

	std::wstring::size_type pos = std::wstring::npos;
	while (std::wstring::npos != (pos = str.find(_T('\\'), pos + 1)))
	{
		++pos;
		str.insert(pos, _T("\\"));
	};
}

void tools::clear_folder(CString szPath)
{
	CFileFind ff;
	CString path = szPath;

	if(path.Right(1) != "\\")
		path += "\\";

	path += "*.*";

	BOOL res = ff.FindFile(path);

	while(res)
	{
		res = ff.FindNextFile();

		if (TRUE == ff.IsDots())
			continue;

		if (FALSE == ff.IsDirectory())
			DeleteFile(ff.GetFilePath());
		else 
		{
			path = ff.GetFilePath();
			clear_folder(path);
			::RemoveDirectory(path);
		}
	}
}

BOOL tools::create_sub_folders( CString szFilePath )
{
	if (szFilePath.IsEmpty())
		return FALSE;

	INT pos_slash = szFilePath.ReverseFind(_T('\\'));

	szFilePath.Delete(pos_slash, szFilePath.GetLength() - pos_slash);

	DWORD attributes = GetFileAttributes(szFilePath);	
	if (0xFFFFFFFF ==  attributes)
	{
		INT res = SHCreateDirectoryEx(nullptr, szFilePath, nullptr);
		if (ERROR_SUCCESS != res)
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL tools::split_path( IN const std::wstring& path, OUT std::wstring* p_drive /*= NULL*/, OUT std::wstring* p_dir /*= NULL*/, OUT std::wstring* p_name /*= NULL*/, OUT std::wstring* p_ext /*= NULL*/, OUT std::wstring* p_name_and_ext /*= NULL*/ )
{
	wchar_t drive[ _MAX_DRIVE ];
	wchar_t dir[ _MAX_DIR ];
	wchar_t fname[ _MAX_FNAME ];
	wchar_t ext[ _MAX_EXT ];

	std::wstring temp = _T("");

	if (0 != _wsplitpath_s( path.c_str(), drive, dir, fname, ext))
		return FALSE;

	if (p_drive)
		*p_drive = drive;

	if (p_dir)
		*p_dir = dir;

	if (p_name)
		*p_name = fname;

	if (p_ext)
		*p_ext = ext;

	if (p_name_and_ext)
	{
		temp = fname;
		temp += ext;
		*p_name_and_ext = temp;
	}
	return TRUE;
}

BOOL tools::get_module_directory( OUT std::wstring& path )
{
	wchar_t m_File[ _MAX_PATH ]; // file name
	std::wstring drive;
	std::wstring dir;

	if(FALSE == GetModuleFileName( nullptr, m_File, _countof( m_File ) - 1 ) )
	{
		return FALSE;
	}
	else
	{
		split_path(m_File, &drive, &dir);
	}

	path = drive.c_str();
	path += dir.c_str();

	return TRUE;
}

BOOL tools::get_module_directory( OUT CString& path )
{
	std::wstring std_path(path.GetString());

	if (FALSE == get_module_directory(std_path))
		return FALSE;

	path = std_path.c_str();
	return TRUE;
}

BOOL tools::add_time_bias( COleDateTime& date_time )
{
	if (COleDateTime::valid != date_time.GetStatus())
		return FALSE;

	// вычитаем из времени величину часового пояса
	COleDateTimeSpan dts;
	TIME_ZONE_INFORMATION tzi;
	GetTimeZoneInformation(&tzi);
	dts.SetDateTimeSpan(0, tzi.Bias / 60, 0, 0);
	date_time += dts;

	return TRUE;
}

std::wstring tools::string_to_wstring( const std::string& str )
{
	std::locale loc("Russian");

	const std::size_t len = str.length() + 1;

	char* buf = new char[len];
	ZeroMemory(buf, len * sizeof(char));
	strcpy_s(buf, len, str.c_str());

	wchar_t* wbuf = new wchar_t[len];
	ZeroMemory(wbuf, len * sizeof(wchar_t));

	std::use_facet<std::ctype<wchar_t> >(loc).widen(buf, buf + len, wbuf);

	std::wstring result(wbuf);

	delete[] buf;
	delete[] wbuf;

	return result;
}

std::string tools::wstring_to_string( const std::wstring& str )
{
	std::locale loc("Russian");

	const std::size_t len = str.length() + 1;

	char* buf = new char[len];
	ZeroMemory(buf, len * sizeof(char));

	wchar_t* wbuf = new wchar_t[len];
	ZeroMemory(wbuf, len * sizeof(wchar_t));
	wcscpy_s(wbuf, len, str.c_str());

	std::use_facet<std::ctype<wchar_t> >(loc).narrow(wbuf, wbuf + len, ' ', buf);

	std::string result(buf);

	delete[] wbuf;
	delete[] buf;

	return result;
}
