#pragma once
#include "stdafx.h"

namespace tools
{

namespace data_wrappers
{
// указатель на буфер данных и его размер
struct _tag_data_const
{
	const byte	*p_data;		// указатель на буфер
	uint32_t	data_size;		// размер данных (не буфера)

	_tag_data_const() : p_data(nullptr), data_size(NULL)
	{
	}

	_tag_data_const(const byte *data, const uint32_t& size)
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

// указатель на данные и размер данных
// производит также выделение и освобождение пам€ти под данные
struct _tag_data_managed
{
protected:
	Concurrency::critical_section _cs;

	// выделение пам€ти дл€ данных
	bool alloc_data(const uint32_t& size)
	{
		free_data();
		if (0 == size)
			return false;

		p_data = new byte[size];
		if (nullptr != p_data)
			data_size = size;

		return (nullptr != p_data);
	}

public:
	byte		*p_data;				// указатель на буфер
	uint32_t	data_size;				// размер буфера
	bool		free_after_destruct;	// освобождать после деструкции

	_tag_data_managed() : p_data(nullptr), data_size(0), free_after_destruct(true)
	{

	}

	_tag_data_managed(const _tag_data_managed& ob) : p_data(nullptr), data_size(0), free_after_destruct(true)
	{
		copy_data_inside(ob.p_data, ob.data_size);
	}

	~_tag_data_managed()
	{
		if (true == free_after_destruct)
			free_data();
	}

	_tag_data_managed(const void *data, const uint32_t& size) : p_data(nullptr), data_size(NULL), free_after_destruct(true)
	{
		copy_data_inside(data, size);
	}

	_tag_data_managed(const _tag_data_const& const_data) : p_data(nullptr), data_size(NULL), free_after_destruct(true)
	{
		copy_data_inside(const_data.p_data, const_data.data_size);
	}

	// копирование данных в экземпл€р
	BOOL copy_data_inside(const void *from, const uint32_t& size)
	{
		Concurrency::critical_section::scoped_lock locker(_cs);
		if (nullptr == from)
		{
			data_size = size;
			return TRUE;
		}

		if (FALSE == alloc_data(size))
			return FALSE;

		return (0 == memcpy_s(p_data, data_size, from, size));
	}

	// освобождение пам€ти дл€ данных
	void free_data()
	{
		delete[] p_data;
		// Concurrency::Free(p_data);
		p_data = nullptr;
		data_size = 0;
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
