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

        public e_convert_result ParseIdentificationPacket(tag_transport_packet data_to_parse, out tag_identification_packet result_packet)
        {
            result_packet = new tag_identification_packet();

            if (e_packet_type.id != data_to_parse.type)
                return e_convert_result.invalid_data;

            Byte[] data = data_to_parse.data;

            if (40 != data.Length)
                return e_convert_result.invalid_data;

            result_packet.group_name = data.SubArray(0, 20).ToCharArray();
            result_packet.terminal_name = data.SubArray(20, 20).ToCharArray();

            return e_convert_result.success;
        }


    }
}
