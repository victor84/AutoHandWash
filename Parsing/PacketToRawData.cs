using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Parsing
{
    public class PacketToRawData
    {

        // создание данных для отправки из пакета транспортного уровня
	    public e_convert_result CreateRawData(tag_transport_packet packet, out Byte[] result_data)
        {
            List<Byte> collection = new List<Byte>();

            collection.AddRange(BitConverter.GetBytes(packet.begin));
            collection.Add((Byte)packet.type);
            collection.AddRange(BitConverter.GetBytes(packet.length));
            collection.AddRange(packet.data);
            collection.AddRange(BitConverter.GetBytes(packet.end));

            result_data = collection.ToArray();

            return e_convert_result.success;
        }

        // создание данных для отправки из пакета идентификации
        public e_convert_result CreateIdentificationPacketRawData(tag_identification_packet packet, out Byte[] result_data)
        {
            List<Byte> collection = new List<Byte>();

            collection.AddRange(packet.group_name.ToByteArray());
            collection.AddRange(packet.terminal_name.ToByteArray());

            result_data = collection.ToArray();
            return e_convert_result.success;
        }

        // создание данных для отправки из пакета счётчиков
        public e_convert_result CreateCountersPacketRawData(tag_counters_packet packet, out Byte[] result_data)
        {
            List<Byte> collection = new List<Byte>();

            collection.AddRange(BitConverter.GetBytes(packet.date_time));
            collection.AddRange(BitConverter.GetBytes(packet.total_cache));
            collection.AddRange(BitConverter.GetBytes(packet.current_cache));
            collection.Add((Byte)packet.state);
            collection.AddRange(BitConverter.GetBytes(packet.pressurized_water));
            collection.AddRange(BitConverter.GetBytes(packet.water_without_pressure));
            collection.AddRange(BitConverter.GetBytes(packet.foam));
            collection.AddRange(BitConverter.GetBytes(packet.wax));
            collection.AddRange(BitConverter.GetBytes(packet.against_midges));
            collection.AddRange(BitConverter.GetBytes(packet.vacuum_cleaner));
            collection.AddRange(BitConverter.GetBytes(packet.air));
            collection.AddRange(BitConverter.GetBytes(packet.osmosis));

            result_data = collection.ToArray();
            return e_convert_result.success;
        }

        // создание данных для отправки из пакета настроек
        public e_convert_result CreateSettingsPacketRawData(tag_settings_packet packet, out Byte[] result_data)
        {
            List<Byte> collection = new List<Byte>();

            collection.Add((Byte)packet.state);
            collection.Add(packet.bill_acceptor_impulse);
            collection.Add(packet.coin_acceptor_impulse);
            collection.Add(packet.free_idle_time);
            collection.AddRange(BitConverter.GetBytes(packet.idle_time_cost));
            collection.Add(packet.pause_before_advertising);
            collection.AddRange(BitConverter.GetBytes(packet.pressurized_water));
            collection.AddRange(BitConverter.GetBytes(packet.water_without_pressure));
            collection.AddRange(BitConverter.GetBytes(packet.foam));
            collection.AddRange(BitConverter.GetBytes(packet.wax));
            collection.AddRange(BitConverter.GetBytes(packet.against_midges));
            collection.AddRange(BitConverter.GetBytes(packet.vacuum_cleaner));
            collection.AddRange(BitConverter.GetBytes(packet.air));
            collection.AddRange(BitConverter.GetBytes(packet.osmosis));

            result_data = collection.ToArray();
            return e_convert_result.success;
        }

        // создание данных для отправки из пакета настроек
        public e_convert_result CreateConfirmationPacketRawData(tag_confirmation_packet packet, out Byte[] result_data)
        {
            result_data = new Byte[2];

            result_data[0] = (Byte)packet.packet_type;
            result_data[1] = (Byte)packet.result;

            return e_convert_result.success;
        }

        // создание данных для отправки из пакета пополнения баланса
        public e_convert_result CreateRefillCachePacketRawData(tag_refill_cache_packet packet, out Byte[] result_data)
        {
            result_data = BitConverter.GetBytes(packet.cache);

            return e_convert_result.success;
        }

        // создание данных для отправки из пакета приза
        public e_convert_result CreatePrizePacketRawData(tag_prize_packet packet, out Byte[] result_data)
        {
            result_data = BitConverter.GetBytes(packet.prize_size);

            return e_convert_result.success;
        }

        // создание данных для отправки из пакета настроек
        public e_convert_result CreateLogRecordPacketRawData(tag_log_record_packet packet, out Byte[] result_data)
        {
            List<Byte> collection = new List<Byte>();

            collection.AddRange(BitConverter.GetBytes(packet.date_time));
            collection.Add((Byte)packet.type);
            collection.AddRange(BitConverter.GetBytes(packet.length));

            foreach (Char c in packet.text)
            {
                collection.AddRange(BitConverter.GetBytes(c));
            }

            result_data = collection.ToArray();

            return e_convert_result.success;
        }


    }
}
