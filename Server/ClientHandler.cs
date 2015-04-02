using Parsing;
using Server.Data;
using System;
using System.Linq;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;
using Server.Hubs;

namespace Server
{
    public class ClientHandler
    {
        private const int size = 0x10000;
        private Group group;
        private Terminal terminal;
        private TcpClient client;
        private NetworkStream stream;
        private PacketParser parser;
        private PacketToRawData packetToRawData;
        private HubTerminals hubTerminals;
        public ClientHandler(TcpClient client, IHubClient hubClient)
        {
            this.client = client;
            this.parser = new PacketParser();
            packetToRawData = new PacketToRawData();
            hubTerminals = new HubTerminals(hubClient);
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
                    catch (Exception e)
                    {
                        Console.WriteLine("ClientHandler -> Handle -> Loop: {0}", e);
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

        private e_processing_result HandleId(tag_transport_packet packet)
        {
            tag_identification_packet identificationPacket;

            if (e_convert_result.success != parser.ParseIdentificationPacket(packet, out identificationPacket))
                return e_processing_result.failed;

            string groupName = new string(identificationPacket.group_name);
            string terminalName = new string(identificationPacket.terminal_name);

            groupName = groupName.Remove(groupName.IndexOf('\0'));
            terminalName = terminalName.Remove(terminalName.IndexOf('\0'));

            group = Group.GetGroupByName(groupName);
            if (group == null)
            {
                Group newGroup = new Group()
                {
                    Id = Guid.NewGuid(),
                    GroupName = groupName
                };

                if (!Group.Insert(newGroup))
                    return e_processing_result.failed;

                group = newGroup;
            }

            IEnumerable<Terminal> terminals = Terminal.GetTerminalsByName(terminalName);
            if (terminals == null || !terminals.Any())
            {
                Terminal newTerminal = new Terminal()
                {
                    Id = Guid.NewGuid(),
                    TerminalName = terminalName,
                    GroupId = group.Id
                };

                if (!Terminal.Insert(newTerminal))
                    return e_processing_result.failed;

                terminal = newTerminal;
            }
            else
            {
                terminal = terminals.Where(x => x.GroupId == group.Id).FirstOrDefault();
            }

            return e_processing_result.success;
        }

        private e_processing_result HandleCounters(tag_transport_packet packet)
        {
            tag_counters_packet countersPacket;

            if (e_convert_result.success != parser.ParseCountersPacket(packet, out countersPacket))
                return e_processing_result.failed;

            Counters counters = new Counters()
            {
                Id = Guid.NewGuid(),
                TerminalId = terminal.Id,
                AgainstOfMidges = countersPacket.against_midges,
                Air = countersPacket.air,
                Balance = countersPacket.total_cache,
                CommonInput = countersPacket.total_cache,
                DateTimeServerEvent = DateTime.Now,
                DateTimeTerminalEvent = Tools.UnixTimeStampToDateTime(countersPacket.date_time),
                Foam = countersPacket.foam,
                NoPressurizedWater = countersPacket.water_without_pressure,
                Osmose = countersPacket.osmosis,
                PressurizedWater = countersPacket.pressurized_water,
                State = (byte)countersPacket.state,
                Vacuum = countersPacket.vacuum_cleaner,
                Wax = countersPacket.wax
            };

            if (!Counters.Insert(counters))
                return e_processing_result.failed;

            hubTerminals.Send("Craig", "12345");

            return e_processing_result.success;
        }

        private e_processing_result HandleSettings(tag_transport_packet packet)
        {
            tag_settings_packet settings_packet;

            if (e_convert_result.success != parser.ParseSettingsPacket(packet, out settings_packet))
                return e_processing_result.failed;

            SettingsTerminal newSettingsTerminal = new SettingsTerminal()
            {
                TerminalId = terminal.Id,
                ImpulseBillAcceptor = settings_packet.bill_acceptor_impulse,
                ImpulseCoinAcceptor = settings_packet.coin_acceptor_impulse,
                PauseBeforeShowingAds = settings_packet.pause_before_advertising,
                PriceAgainstOfMidges = settings_packet.against_midges,
                PriceAir = settings_packet.air,
                PriceFoam = settings_packet.foam,
                PriceMinuteInactivity = settings_packet.idle_time_cost,
                PriceNoPressurizedWater = settings_packet.water_without_pressure,
                PricePressurizedWater = settings_packet.pressurized_water,
                PriceVacuum = settings_packet.vacuum_cleaner,
                PriceWax = settings_packet.wax,
                PriceOsmose = settings_packet.osmosis,
                State = (byte)settings_packet.state,
                TimeInactivity = settings_packet.free_idle_time
            };

            var settingsTerminal = SettingsTerminal.GetSettingsTerminalById(terminal.Id);
            bool result;
            if (settingsTerminal != null)
            {
                result = SettingsTerminal.Update(newSettingsTerminal);
            }
            else
            {
                result = SettingsTerminal.Insert(newSettingsTerminal);
            }

            if (!result)
                return e_processing_result.failed;

            return e_processing_result.success;
        }

        private e_processing_result HandleLogRecordPacket(tag_transport_packet packet)
        {
            tag_log_record_packet log_record_packet;

            if (e_convert_result.success != parser.ParseLogRecordPacket(packet, out log_record_packet))
                return e_processing_result.failed;

            TerminalLog terminalLog = new TerminalLog() 
            { 
                Id = Guid.NewGuid(),
                TerminalId = terminal.Id,
                DateTimeTerminal = Tools.UnixTimeStampToDateTime(log_record_packet.date_time),
                MessageType = (byte)log_record_packet.type,
                Message = new string(log_record_packet.text)
            };

            if (!TerminalLog.Insert(terminalLog))
                return e_processing_result.failed;

            hubTerminals.Send("Craig", "12345");

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
