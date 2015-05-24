#pragma once
#include "device_protocol.h"
#include "raw_data_warappers.h"
#include "TraceError.h"

namespace device_exchange
{
// Конвертор сообщений от/для устройства
template<typename _PacketType>
class CDevicePacketConvertor
{
	tools::logging::CTraceError* _tr_error;

public:
	CDevicePacketConvertor()
	{
		_tr_error = tools::logging::CTraceError::get_instance();
	}
	virtual ~CDevicePacketConvertor()
	{

	}

	byte CalcCheckSum(IN const _PacketType& packet)
	{
		return static_cast<byte>(packet.command) ^
			packet.data0 ^
			packet.data1 ^
			packet.data2 ^
			packet.data3 ^
			packet.data4 ^
			packet.data5;
	}


	// парсинг пакета с устройства
	e_convert_result Parse(IN const tools::data_wrappers::_tag_data_const& data,
						   OUT std::vector<_PacketType>& packets)
	{
		const int16_t packet_size = sizeof(tag_packet_from_device);

		if (data.data_size >= packet_size)
		{
			_tr_error->trace_error(_T("Неполные данные от устройства"));
			return e_convert_result::invalid_data;
		}

		packets.clear();
		const int16_t count = static_cast<int16_t>(data.data_size / packet_size);

		for (int16_t i = 0; i < count; ++i)
		{
			const tag_packet_from_device* p_packet = reinterpret_cast<const tag_packet_from_device*>(data.p_data + (packet_size * i));

			if (p_packet->crc != CalcCheckSum(*p_packet))
			{
				_tr_error->trace_error(_T("Неверная контрольная сумма в паркете"));
				continue;
			}
			packets.push_back(*p_packet);
		}

		return e_convert_result::success;
	}

	// Парсинг с вычислением и записью в пакет контрольной суммы
	e_convert_result ParseWithUpdateCheckSum(IN const tools::data_wrappers::_tag_data_const& data,
										   OUT _PacketType& packet)
	{
		if (data.data_size != sizeof(tag_packet_from_device))
			return e_convert_result::invalid_data;

		packet = *(reinterpret_cast<const tag_packet_from_device*>(data.p_data));

		packet.begin = begin_byte;
		packet.end = end_byte;

		packet.crc = CalcCheckSum(packet);

		return e_convert_result::success;
	}

	// получение сырых данных из пакета
	// считает и пишет контрольную сумму
	e_convert_result CreateRawData(IN OUT _PacketType& packet, OUT tools::data_wrappers::_tag_data_managed& data)
	{
		packet.crc = CalcCheckSum(packet);
		data.copy_data_inside(static_cast<void*>(&packet), sizeof(_PacketType));

		return e_convert_result::success;
	}
};
}



