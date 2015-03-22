#pragma once
#include "stdafx.h"

namespace tools
{
template<typename _ElemType>
class lock_deque
{
	std::deque<_ElemType> _deque;
	Concurrency::critical_section _cs;

public:

	void push_back(_ElemType elem)
	{
		Concurrency::critical_section::scoped_lock sl(_cs);

		_deque.push_back(elem);
	}

	void push_front(_ElemType elem)
	{
		Concurrency::critical_section::scoped_lock sl(_cs);

		_deque.push_front(elem);
	}

	bool empty()
	{
		Concurrency::critical_section::scoped_lock sl(_cs);

		return _deque.empty();
	}

	std::vector<_ElemType> get_with_cleanup()
	{
		Concurrency::critical_section::scoped_lock sl(_cs);

		std::vector<_ElemType> result(_deque.begin(), _deque.end());

		_deque.clear();

		return result;
	}

};


}
