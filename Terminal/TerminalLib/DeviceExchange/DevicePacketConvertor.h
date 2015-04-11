#pragma once
#include "device_protocol.h"
#include "raw_data_warappers.h"

namespace device_exchange
{
//  онвертор сообщений от/дл€ устройства
template<typename _PacketType>
class CDevicePacketConvertor
{
public:
	CDevicePacketConvertor()
	{

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
						   OUT _PacketType& packet)
	{
		if (data.data_size != sizeof(tag_packet_from_device))
			return e_convert_result::invalid_data;

		packet = *(reinterpret_cast<const tag_packet_from_device*>(data.p_data));

		if (packet.crc != CalcCheckSum(packet))
			return e_convert_result::invalid_data;

		return e_convert_result::success;
	}

	// ѕарсинг с вычислением и записью в пакет контрольной суммы
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



