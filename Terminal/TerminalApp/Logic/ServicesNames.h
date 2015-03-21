#pragma once
#include "stdafx.h"

/*!
 * \file ServicesNames.h
 *
 * \author Victor Mulika
 * \date 20 Март 2015
 *
 * 
 */

namespace logic
{
	// название сервиса
	enum class e_service_name
	{
		pressurized_water,		// Вода под давлением
		water_without_pressure,	// Вода без давления
		foam,					// Пена	
		wax,					// Воск	
		against_midges,			// От мошек
		vacuum_cleaner,			// Пылесос	
		air,					// Воздух
		osmosis					// Осмос
	};
}
