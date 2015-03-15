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
        private const int size = 1024;
        private TcpClient client;
        private NetworkStream stream;
        private PacketParser parser;
        public ClientHandler(TcpClient client)
        {
            this.client = client;
            this.parser = new PacketParser();
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
                        byte[] bytes = new byte[size];
                        int i = 0;
                        while ((i = stream.Read(bytes, 0, bytes.Length)) != 0)
                        {
                            IEnumerable<tag_transport_packet> transport_packets;
                            var result = parser.ParseTransportPacket(bytes, out transport_packets);
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
            tag_confirmation_packet confirmation_packet = new tag_confirmation_packet();
            confirmation_packet.packet_type = packet_type;
            confirmation_packet.result = result;
            byte[] bytes = new byte[size];
            stream.Write(bytes, 0, bytes.Length);
        }

        private e_processing_result HandleCounters(tag_transport_packet packet)
        {
            return e_processing_result.success;
        }

        private e_processing_result HandleId(tag_transport_packet packet)
        {
            return e_processing_result.success;
        }

        private e_processing_result HandleSettings(tag_transport_packet packet)
        {
            return e_processing_result.success;
        }
    }
}
