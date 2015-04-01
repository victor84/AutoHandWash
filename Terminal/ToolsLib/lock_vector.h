#pragma once
#include "CriticalSectionLocker.h"

namespace tools
{
template<typename _Type>
class lock_vector
{
	std::vector<_Type> _vector;
	CCriticalSection _cs;

public:
	lock_vector()
	{

	}

	~lock_vector()
	{

	}

	void push_back(_Type elem)
	{
		threading::CCriticalSectionLocker sl(_cs);

		_vector.push_back(elem);
	}

	std::vector<_Type> get_with_cleanup()
	{
		threading::CCriticalSectionLocker sl(_cs);

		std::vector<_Type> result(_vector.begin(), _vector.end());

		_vector.clear();
		
		return result;
	}

	bool empty()
	{
		threading::CCriticalSectionLocker sl(_cs);

		return _vector.empty();
	}

};
}



