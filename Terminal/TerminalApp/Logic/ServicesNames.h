#pragma once
#include "stdafx.h"

/*!
 * \file ServicesNames.h
 *
 * \author Victor Mulika
 * \date 20 ���� 2015
 *
 * 
 */

namespace logic
{
	// �������� �������
	enum class e_service_name
	{
		pressurized_water,		// ���� ��� ���������
		water_without_pressure,	// ���� ��� ��������
		foam,					// ����	
		wax,					// ����	
		against_midges,			// �� �����
		vacuum_cleaner,			// �������	
		air,					// ������
		osmosis					// �����
	};
}
