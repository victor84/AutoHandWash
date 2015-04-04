#pragma once

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
	osmosis,				// Осмос
	stop					// Стоп
};


// состояние работы логики
enum class e_state
{
	advertising_idle,	// рекламный простой
	refill_cache,		// денежный баланс пополнен
	executing_service,	// выполнение службы (услуги)
	free_idle,			// бесплатный простой
	paid_idle,			// платный простой
	settings_work,		// работа с настройками
};
}
