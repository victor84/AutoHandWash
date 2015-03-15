using Parsing;
using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;

namespace Server
{
    public class ClientHandler
    {
        private const int size = 0x10000;
        private TcpClient client;
        private NetworkStream stream;
        private PacketParser parser;
        private PacketToRawData packetToRawData;
        public ClientHandler(TcpClient client)
        {
            this.client = client;
            this.parser = new PacketParser();
            packetToRawData = new PacketToRawData();
        }

        public void Run()
        {
            Task.Factory.StartNew(() => Handle());
        }

        private void Handle()
        {
            try
            {
                stream = client.GetStream();
                while (true)
                {
                    try
                    {
                        byte[] receiveBuffer = new byte[size];
                        Int32 readCount = 0;
                        while ((readCount = stream.Read(receiveBuffer, 0, receiveBuffer.Length)) != 0)
                        {
                            Byte[] data = receiveBuffer.SubArray(0, (UInt32)readCount);
                            IEnumerable<tag_transport_packet> transport_packets;
                            var result = parser.ParseTransportPacket(data, out transport_packets);
                            if (result != e_convert_result.success)
                            {
                                Confirmation(e_packet_type.unknown, e_processing_result.failed);
                            }
                            else
                            {
                                foreach (var packet in transport_packets)
                                {
                                    e_processing_result processing_result = e_processing_result.failed;
                                    switch (packet.type)
                                    {
                                        case e_packet_type.counters:
                                            processing_result = HandleCounters(packet);
                                            break;
                                        case e_packet_type.id:
                                            processing_result = HandleId(packet);
                                            break;
                                        case e_packet_type.settings:
                                            processing_result = HandleSettings(packet);
                                            break;
                                        case e_packet_type.log:
                                            processing_result = HandleLogRecordPacket(packet);
                                            break;
                                        case e_packet_type.confirmation:
                                            processing_result = HandleConfirmationPacket(packet);
                                            break;
                                        case e_packet_type.unknown:
                                            break;
                                    }
                                    Confirmation(packet.type, processing_result);
                                }
                            }
                        }
                        Thread.Sleep(1000);
                    }
                    catch
                    {
                        break;
                    }
                }
                client.Close();
            }
            catch (Exception e)
            {
                Console.WriteLine("ClientHandler -> Handle: {0}", e);
            }
        }

        private void Confirmation(e_packet_type packet_type, e_processing_result result)
        {
            tag_confirmation_packet confirmation_packet;
            tag_transport_packet transport_packet = new tag_transport_packet();
            confirmation_packet.packet_type = packet_type;
            confirmation_packet.result = result;

            transport_packet.type = e_packet_type.confirmation;
            packetToRawData.CreateConfirmationPacketRawData(confirmation_packet, out transport_packet.data);
            transport_packet.set_missing_values();

            byte[] bytes;
            packetToRawData.CreateRawData(transport_packet, out bytes);

            stream.Write(bytes, 0, bytes.Length);
        }

        private e_processing_result HandleCounters(tag_transport_packet packet)
        {
            tag_counters_packet countersPacket;

            if (e_convert_result.success != parser.ParseCountersPacket(packet, out countersPacket))
                return e_processing_result.failed;

            // TODO: обработка пакета с показаниями счётчиков

            return e_processing_result.success;
        }

        private e_processing_result HandleId(tag_transport_packet packet)
        {
            tag_identification_packet identificationPacket;

            if (e_convert_result.success != parser.ParseIdentificationPacket(packet, out identificationPacket))
                return e_processing_result.failed;

            // TODO: обработка пакета идентификации

            return e_processing_result.success;
        }

        private e_processing_result HandleSettings(tag_transport_packet packet)
        {
            tag_settings_packet settings_packet;

            if (e_convert_result.success != parser.ParseSettingsPacket(packet, out settings_packet))
                return e_processing_result.failed;

            // TODO: обработка пакета с настройками от терминала

            return e_processing_result.success;
        }

        private e_processing_result HandleLogRecordPacket(tag_transport_packet packet)
        {
            tag_log_record_packet log_record_packet;

            if (e_convert_result.success != parser.ParseLogRecordPacket(packet, out log_record_packet))
                return e_processing_result.failed;

            // TODO: обработка пакета с настройками от терминала

            return e_processing_result.success;
        }

        private e_processing_result HandleConfirmationPacket(tag_transport_packet packet)
        {
            tag_confirmation_packet confirmation_packet;

            if (e_convert_result.success != parser.ParseConfirmationPacket(packet, out confirmation_packet))
                return e_processing_result.failed;

            // TODO: обработка пакета с настройками от терминала

            return e_processing_result.success;
        }


    }
}
