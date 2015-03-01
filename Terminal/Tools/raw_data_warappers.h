#pragma once
#include "stdafx.h"

namespace tools
{

namespace data_wrappers
{
// ��������� �� ����� ������ � ��� ������
struct _tag_data_const
{
	const BYTE	*p_data;		// ��������� �� �����
	SIZE_T		data_size;		// ������ ������ (�� ������)

	_tag_data_const() : p_data(nullptr), data_size(NULL)
	{
	}

	_tag_data_const(const BYTE *data, const SIZE_T& size)
	{
		p_data = data;
		data_size = size;
	}

	operator const PVOID()
	{
		return (const PVOID)(p_data);
	}

	void free_data()
	{
		delete p_data;
		p_data = nullptr;
		data_size = 0;
	}

};

// ��������� �� ������ � ������ ������
// ���������� ����� ��������� � ������������ ������ ��� ������
struct _tag_data_managed
{
protected:
	Concurrency::critical_section _cs;

	// ��������� ������ ��� ������
	BOOL alloc_data(const SIZE_T& size)
	{
		// ASSERT(NULL != size);
		free_data();
		if (NULL == size)
			return FALSE;

		p_data = new BYTE[size];
		// p_data = reinterpret_cast<BYTE*>(Concurrency::Alloc(data_size));
		if (nullptr != p_data)
			data_size = size;

		return (nullptr != p_data);
	}

public:
	BYTE	*p_data;				// ��������� �� �����
	SIZE_T	data_size;				// ������ ������
	bool	free_after_destruct;	// ����������� ����� ����������

	_tag_data_managed() : p_data(nullptr), data_size(NULL), free_after_destruct(true)
	{

	}

	_tag_data_managed(const _tag_data_managed& ob) : p_data(nullptr), data_size(NULL), free_after_destruct(true)
	{
		copy_data_inside(ob.p_data, ob.data_size);
	}

	~_tag_data_managed()
	{
		if (true == free_after_destruct)
			free_data();
	}

	_tag_data_managed(const VOID *data, const SIZE_T& size) : p_data(nullptr), data_size(NULL), free_after_destruct(true)
	{
		copy_data_inside(data, size);
	}

	_tag_data_managed(const _tag_data_const& const_data) : p_data(nullptr), data_size(NULL), free_after_destruct(true)
	{
		copy_data_inside(const_data.p_data, const_data.data_size);
	}

	// ����������� ������ � ���������
	BOOL copy_data_inside(const VOID *from, const SIZE_T& size)
	{
		Concurrency::critical_section::scoped_lock locker(_cs);
		if (nullptr == from)
		{
			data_size = size;
			return TRUE;
		}

		if (FALSE == alloc_data(size))
			return FALSE;

		return (NULL == memcpy_s(p_data, data_size, from, size));
	}

	// ������������ ������ ��� ������
	void free_data()
	{
		delete[] p_data;
		// Concurrency::Free(p_data);
		p_data = nullptr;
		data_size = NULL;
	}

	_tag_data_managed& operator=(const _tag_data_managed& ob)
	{
		copy_data_inside(ob.p_data, ob.data_size);
		return *this;
	}

	operator _tag_data_const()
	{
		return _tag_data_const(p_data, data_size);
	}

};
}
}
