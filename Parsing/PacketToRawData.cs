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
    }
}
