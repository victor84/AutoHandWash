using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Parsing
{
    public class PacketParser
    {
        private e_convert_result get_transport_packet(ref UInt32 offset, 
														   Byte[] data,
														   out tag_transport_packet result_packet)
        {
            result_packet = new tag_transport_packet();

	        if ((data.Length - offset) < 9)
		        return e_convert_result.invalid_data;

            result_packet = new tag_transport_packet();

            result_packet.begin = BitConverter.ToUInt16(new Byte[]{ data[offset], data[++offset] }, 0);

            if (tag_constants.begin_bytes != result_packet.begin)
                return e_convert_result.invalid_data;

	        result_packet.type = (e_packet_type)(data[++offset]);

	        result_packet.length = BitConverter.ToUInt32(new Byte[]{ data[++offset],
                                                                     data[++offset],
                                                                     data[++offset],
                                                                     data[++offset]}, 0);

            result_packet.data = new Byte[result_packet.length];

            Array.Copy(data, ++offset, result_packet.data, 0, result_packet.length);

	        offset += result_packet.length;

	        result_packet.end = BitConverter.ToUInt16(new Byte[]{ data[offset], data[++offset] }, 0);
            ++offset;

	        if (tag_constants.end_bytes != result_packet.end)
		        return e_convert_result.invalid_data;

	        return e_convert_result.success;
        }


        public e_convert_result ParseTransportPacket(Byte[] data, out IEnumerable<tag_transport_packet> result_packets)
        {
            result_packets = null;

            if ((null == data) || (false == data.Any()))
                return e_convert_result.empty_data;

            if (data.Length < 9)
                return e_convert_result.invalid_data;

            UInt32 offset = 0;

	        e_convert_result result = e_convert_result.success;

            List<tag_transport_packet> packets = new List<tag_transport_packet>();

	        while (offset < data.Length)
	        {
		        tag_transport_packet new_packet;

		        if (e_convert_result.invalid_data == get_transport_packet(ref offset, data, out new_packet))
		        {
			        result = e_convert_result.invalid_data;
			        break;
		        }
		        packets.Add(new_packet);
	        }

            result_packets = packets.ToArray();

	        return result;
        }

        // парсинг пакета идентификации
        public e_convert_result ParseIdentificationPacket(tag_transport_packet data_to_parse, out tag_identification_packet result_packet)
        {
            result_packet = new tag_identification_packet();

            if (e_packet_type.id != data_to_parse.type)
                return e_convert_result.invalid_data;

            Byte[] data = data_to_parse.data;

            if (80 != data.Length)
                return e_convert_result.invalid_data;

            result_packet.group_name = UnicodeByteArrayToCharArray(data.SubArray(0, 40));
            result_packet.terminal_name = UnicodeByteArrayToCharArray(data.SubArray(40, 40));

            return e_convert_result.success;
        }

        // парсинг пакета со счётчиками
        public e_convert_result ParseCountersPacket(tag_transport_packet data_to_parse, out tag_counters_packet result_packet)
        {
            result_packet = new tag_counters_packet();

            if (e_packet_type.counters != data_to_parse.type)
                return e_convert_result.invalid_data;

            Byte[] data = data_to_parse.data;

            if (45 != data.Length)
                return e_convert_result.invalid_data;

            UInt16 offset = 0;

            result_packet.date_time = BitConverter.ToUInt64(data.SubArray(offset, 8), 0);
            offset += 8;

            result_packet.total_cache = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            result_packet.current_cache = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            result_packet.state = (e_terminal_state)data[offset++];

            result_packet.pressurized_water = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            result_packet.water_without_pressure = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            result_packet.foam = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            result_packet.wax = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            result_packet.against_midges = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            result_packet.vacuum_cleaner = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            result_packet.air = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            return e_convert_result.success;
        }

        // парсинг пакета со счётчиками
        public e_convert_result ParseSettingsPacket(tag_transport_packet data_to_parse, out tag_settings_packet result_packet)
        {
            result_packet = new tag_settings_packet();

            if (e_packet_type.settings != data_to_parse.type)
                return e_convert_result.invalid_data;

            Byte[] data = data_to_parse.data;

            if (35 != data.Length)
                return e_convert_result.invalid_data;

            UInt16 offset = 0;

            result_packet.state = (e_terminal_state)data[offset++];
            result_packet.bill_acceptor_impulse = data[offset++];
            result_packet.coin_acceptor_impulse = data[offset++];
            result_packet.free_idle_time = data[offset++];
            result_packet.idle_time_cost = BitConverter.ToUInt16(data.SubArray(offset, 2), 0);
            offset += 2;

            result_packet.pause_before_advertising = data[offset++];

            result_packet.pressurized_water = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            result_packet.water_without_pressure = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            result_packet.foam = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            result_packet.wax = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            result_packet.against_midges = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            result_packet.vacuum_cleaner = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            result_packet.air = BitConverter.ToUInt32(data.SubArray(offset, 4), 0);
            offset += 4;

            return e_convert_result.success;
        }

        // парсинг пакета со счётчиками
        public e_convert_result ParseConfirmationPacket(tag_transport_packet data_to_parse, out tag_confirmation_packet result_packet)
        {
            result_packet = new tag_confirmation_packet();

            if (e_packet_type.confirmation != data_to_parse.type)
                return e_convert_result.invalid_data;

            Byte[] data = data_to_parse.data;

            if (2 != data.Length)
                return e_convert_result.invalid_data;

            result_packet.packet_type = (e_packet_type)data[0];
            result_packet.result = (e_processing_result)data[1];

            return e_convert_result.success;
        }

        // парсинг пакета со счётчиками
        public e_convert_result ParseLogRecordPacket(tag_transport_packet data_to_parse, out tag_log_record_packet result_packet)
        {
            result_packet = new tag_log_record_packet();

            if (e_packet_type.log != data_to_parse.type)
                return e_convert_result.invalid_data;

            Byte[] data = data_to_parse.data;

            if (data.Length < 12)
                return e_convert_result.invalid_data;

            UInt32 offset = 0;

            result_packet.date_time = BitConverter.ToUInt64(data.SubArray(offset, 8), 0);
            offset += 8;

            result_packet.type = (e_log_record_type)data[offset++];

            result_packet.length = BitConverter.ToUInt16(data.SubArray(offset, 2), 0);
            offset += 2;

            if (data.Length - offset != result_packet.length)
                return e_convert_result.invalid_data;

            result_packet.text = UnicodeByteArrayToCharArray(data.SubArray(offset, result_packet.length));

            return e_convert_result.success;
        }

        private Char[] UnicodeByteArrayToCharArray(Byte[] text_bytes)
        {
            Char[] text_chars = new Char[text_bytes.Length / 2];

            for (int bn = 0, cn = 0; bn < text_bytes.Length; bn += 2, ++cn)
            {
                Int16 uni_code = BitConverter.ToInt16(new Byte[]
                                                        {
                                                            text_bytes[bn], 
                                                            text_bytes[bn + 1] 
                                                        }, 0);

                text_chars[cn] = Convert.ToChar(uni_code);
            }

            return text_chars;
        }

    }
}
