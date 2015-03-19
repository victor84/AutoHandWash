#pragma once
#include "stdafx.h"

/*!
 * \file BaseStructuresConvertor.h
 *
 * \author Victor Mulika
 * \date 19 Март 2015
 *
 * 
 */

namespace logic_structures
{

/*
CBaseStructuresConvertor
базовый шаблонный класс для преобразования пакетов
*/

template<typename _HandlerResultType,
	typename _ConvertFromType,
	typename _KeyType,
	typename _ConvertResultType,
	typename _ThisClassType>
class CBaseStructuresConvertor
{

protected:
	typedef typename std::function<_HandlerResultType(const _ConvertFromType&)> _Handler_Type;
	typedef typename std::pair<_KeyType, _Handler_Type> _Storage_Elem_Type;

	std::map<_KeyType, _Handler_Type> _handlers;

	void add_command_from_device_handler(_KeyType key, _Handler_Type handler)
	{
		_handlers.insert(_Storage_Elem_Type(key, handler));
	}

	virtual void fill_handlers() = 0;

	virtual _ConvertResultType handler_result_to_convert_result(_HandlerResultType& result) = 0;

	virtual _ConvertResultType get_default_result() = 0;

	CBaseStructuresConvertor()
	{
	}

public:

	virtual ~CBaseStructuresConvertor()
	{
	}


	// конвертирует пакет от устройства в структуру для использования в логике
	_ConvertResultType Convert(const _ConvertFromType& packet, const _KeyType& key)
	{
		_Handler_Type handler = _handlers[key];

		if (handler)
		{
			_HandlerResultType result = handler(packet);
			return handler_result_to_convert_result(result);
		}

		return get_default_result();
	}


};

}