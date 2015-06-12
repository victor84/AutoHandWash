using Parsing;
using Server.Data;
using System;
using System.Linq;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Threading.Tasks;
using System.Threading;
using Server.Hubs;
using System.IO;
using System.Collections.Concurrent;
using Server.Prize;

namespace Server
{
    public class TerminalHandler
    {
        private const int size = 0x10000;
        private Group group;
        private TcpClient client;
        private NetworkStream stream;
        private PacketParser parser;
        private PacketToRawData packetToRawData;
        private IHubClient _hubClient;
        private e_packet_type lastPacket;
        private ConcurrentQueue<ServerPacket> queueServerPacket = new ConcurrentQueue<ServerPacket>();

        private uint commonInput;
        public Guid Id { get; set; }
        public Terminal Terminal { get; set; }
        public event EventHandler ConnectionClosed;
        public event EventHandler<InputArgs> CommonInputExtended;
        public event EventHandler PrizeConfirmed;
        public event EventHandler PrizeNoConfirmed;

        private bool SocketConnected()
        {
            Socket s = client.Client;
            bool part1 = s.Poll(1000, SelectMode.SelectRead);
            bool part2 = (s.Available == 0);
            if (part1 && part2)
                return false;
            else
                return true;
        }

        public TerminalHandler(TcpClient client, IHubClient hubClient)
        {
            this.Id = Guid.NewGuid();

            this.lastPacket = e_packet_type.unknown;
            this.client = client;
            this.parser = new PacketParser();
            this.packetToRawData = new PacketToRawData();
            this._hubClient = hubClient;
        }

        public void EnqueueServerPacket(ServerPacket serverPacket)
        {
            queueServerPacket.Enqueue(serverPacket);
        }

        private ServerPacket DequeueServerPacket()
        {
            ServerPacket serverPacket;
            bool success = queueServerPacket.TryDequeue(out serverPacket);
            if (!success)
            {
                return null;
            }
            return serverPacket;
        }

        private void WriteServerPacket(ServerPacket serverPacket)
        {
            var packetType = serverPacket.PacketType;
            switch (packetType)
            {
                case ServerPacketType.settingsTerminal:
                    WriteSettings((SettingsTerminal)serverPacket.Data);
                    break;
                case ServerPacketType.fillcache:
                    WriteCache((UInt16)serverPacket.Data);
                    break;
                case ServerPacketType.changeStatus:
                    WriteStatus((byte)serverPacket.Data);
                    break;
                case ServerPacketType.prize:
                    WritePrize((UInt16)serverPacket.Data);
                    break;
            }
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
                while (SocketConnected())
                {
                    try
                    {
                        byte[] receiveBuffer = new byte[size];
                        Int32 readCount = 0;
                        if (stream.DataAvailable)
                        {
                            readCount = stream.Read(receiveBuffer, 0, receiveBuffer.Length);
                            if (readCount > 0)
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
                                        HandleResult(packet, processing_result);
                                    }
                                }
                            }
                        }
                        else
                        {
                            var serverPacket = DequeueServerPacket();
                            if (serverPacket != null)
                            {
                                WriteServerPacket(serverPacket);
                            }
                        }
                        Thread.Sleep(1000);
                    }
                    catch (IOException)
                    {
                        break;
                    }
                    catch (Exception e)
                    {
                        ServerLogger.Error(string.Format("ClientHandler -> Handle -> Loop: {0}", e.Message));
                        break;
                    }
                }
                client.Close();
            }
            catch (Exception e)
            {
                ServerLogger.Error(string.Format("ClientHandler -> Handle: {0}", e.Message));
            }
            finally
            {
                EventHandler handler = ConnectionClosed;
                if (handler != null)
                {
                    handler(this, new EventArgs());
                }
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

        private void WriteSettings(SettingsTerminal settingsTerminal)
        {
            tag_settings_packet settings_packet;
            settings_packet.state = (e_terminal_state)settingsTerminal.State;
            settings_packet.bill_acceptor_impulse = (byte)settingsTerminal.ImpulseBillAcceptor;
            settings_packet.coin_acceptor_impulse = (byte)settingsTerminal.ImpulseCoinAcceptor;
            settings_packet.free_idle_time = (byte)settingsTerminal.TimeInactivity;
            settings_packet.idle_time_cost = (byte)settingsTerminal.PriceMinuteInactivity;
            settings_packet.osmosis = settingsTerminal.PriceOsmose;
            settings_packet.pause_before_advertising = (byte)settingsTerminal.PauseBeforeShowingAds;
            settings_packet.pressurized_water = settingsTerminal.PricePressurizedWater;
            settings_packet.water_without_pressure = settingsTerminal.PriceNoPressurizedWater;
            settings_packet.foam = settingsTerminal.PriceFoam;
            settings_packet.wax = settingsTerminal.PriceWax;
            settings_packet.vacuum_cleaner = settingsTerminal.PriceVacuum;
            settings_packet.air = settingsTerminal.PriceAir;
            settings_packet.frost_protection_value = settingsTerminal.FrostProtection;
            settings_packet.discount_card_condition = settingsTerminal.ConditionDiscountCard;

            tag_transport_packet transport_packet = new tag_transport_packet();
            transport_packet.type = e_packet_type.settings;
            packetToRawData.CreateSettingsPacketRawData(settings_packet, out transport_packet.data);
            transport_packet.set_missing_values();

            byte[] bytes;
            packetToRawData.CreateRawData(transport_packet, out bytes);

            stream.Write(bytes, 0, bytes.Length);

            lastPacket = e_packet_type.settings;
        }

        private void WriteCache(UInt16 cache)
        {
            tag_refill_cache_packet cache_packet;
            cache_packet.cache = (UInt16)cache;

            tag_transport_packet transport_packet = new tag_transport_packet();
            transport_packet.type = e_packet_type.refill_cache;
            packetToRawData.CreateRefillCachePacketRawData(cache_packet, out transport_packet.data);
            transport_packet.set_missing_values();

            byte[] bytes;
            packetToRawData.CreateRawData(transport_packet, out bytes);

            stream.Write(bytes, 0, bytes.Length);

            lastPacket = e_packet_type.refill_cache;
        }

        private void WriteStatus(byte state)
        {
            tag_terminal_state_packet state_packet;
            state_packet.state = (e_terminal_state)state;

            tag_transport_packet transport_packet = new tag_transport_packet();
            transport_packet.type = e_packet_type.terminal_state;
            packetToRawData.CreateTerminalStatePacketRawData(state_packet, out transport_packet.data);
            transport_packet.set_missing_values();

            byte[] bytes;
            packetToRawData.CreateRawData(transport_packet, out bytes);

            stream.Write(bytes, 0, bytes.Length);

            lastPacket = e_packet_type.terminal_state;
        }

        private void WritePrize(UInt16 prize_size)
        {
            tag_prize_packet prize_packet;
            prize_packet.prize_size = (UInt16)prize_size;

            tag_transport_packet transport_packet = new tag_transport_packet();
            transport_packet.type = e_packet_type.prize;
            packetToRawData.CreatePrizePacketRawData(prize_packet, out transport_packet.data);
            transport_packet.set_missing_values();

            byte[] bytes;
            packetToRawData.CreateRawData(transport_packet, out bytes);

            stream.Write(bytes, 0, bytes.Length);

            lastPacket = e_packet_type.prize;
        }

        private void WriteTerminalState(e_terminal_state state)
        {
            tag_terminal_state_packet terminal_state_packet;
            terminal_state_packet.state = state;

            tag_transport_packet transport_packet = new tag_transport_packet();
            transport_packet.type = e_packet_type.terminal_state;
            packetToRawData.CreateTerminalStatePacketRawData(terminal_state_packet, out transport_packet.data);
            transport_packet.set_missing_values();

            byte[] bytes;
            packetToRawData.CreateRawData(transport_packet, out bytes);

            stream.Write(bytes, 0, bytes.Length);

            lastPacket = e_packet_type.terminal_state;
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

                Terminal = newTerminal;
            }
            else
            {
                Terminal = terminals.Where(x => x.GroupId == group.Id).FirstOrDefault();
            }

            var counters = Counters.GetCountersByTerminal(Terminal.Id);
            if (counters != null)
            {
                commonInput = counters.CommonInput;
            }

            return e_processing_result.success;
        }

        private e_processing_result HandleCounters(tag_transport_packet packet)
        {
            tag_counters_packet countersPacket;

            if (e_convert_result.success != parser.ParseCountersPacket(packet, out countersPacket))
                return e_processing_result.failed;

            Counters newCounters = new Counters()
            {
                Id = Guid.NewGuid(),
                TerminalId = Terminal.Id,
                Air = countersPacket.air,
                Balance = countersPacket.current_cache,
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

            if (!Counters.Insert(newCounters))
                return e_processing_result.failed;

            RefreshCounters(Terminal.TerminalName, newCounters);

            if (newCounters.CommonInput > commonInput)
            {
                EventHandler<InputArgs> handler = CommonInputExtended;
                if (handler != null)
                {
                    var input = newCounters.CommonInput - commonInput;
                    commonInput = newCounters.CommonInput;
                    handler(this, new InputArgs(input));
                }
            }

            return e_processing_result.success;
        }

        private e_processing_result HandleSettings(tag_transport_packet packet)
        {
            tag_settings_packet settings_packet;

            if (e_convert_result.success != parser.ParseSettingsPacket(packet, out settings_packet))
                return e_processing_result.failed;

            SettingsTerminal newSettingsTerminal = new SettingsTerminal()
            {
                TerminalId = Terminal.Id,
                DataSent = true,
                ImpulseBillAcceptor = settings_packet.bill_acceptor_impulse,
                ImpulseCoinAcceptor = settings_packet.coin_acceptor_impulse,
                PauseBeforeShowingAds = settings_packet.pause_before_advertising,
                PriceAir = settings_packet.air,
                PriceFoam = settings_packet.foam,
                PriceMinuteInactivity = settings_packet.idle_time_cost,
                PriceNoPressurizedWater = settings_packet.water_without_pressure,
                PricePressurizedWater = settings_packet.pressurized_water,
                PriceVacuum = settings_packet.vacuum_cleaner,
                PriceWax = settings_packet.wax,
                PriceOsmose = settings_packet.osmosis,
                FrostProtection = settings_packet.frost_protection_value,
                ConditionDiscountCard = settings_packet.discount_card_condition,
                State = (byte)settings_packet.state,
                TimeInactivity = settings_packet.free_idle_time
            };

            var settingsTerminal = SettingsTerminal.GetSettingsTerminalById(Terminal.Id);
            if (settingsTerminal == null)
            {
                if (!SettingsTerminal.Insert(newSettingsTerminal))
                    return e_processing_result.failed;
            }

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
                TerminalId = Terminal.Id,
                DateTimeTerminal = Tools.UnixTimeStampToDateTime(log_record_packet.date_time),
                MessageType = (byte)log_record_packet.type,
                Message = new string(log_record_packet.text)
            };

            if (!TerminalLog.Insert(terminalLog))
                return e_processing_result.failed;

            RefreshStatusBar(Terminal.GroupId, terminalLog);
            RefreshMessages(Terminal.GroupId, Terminal.TerminalName, terminalLog);

            return e_processing_result.success;
        }

        private e_processing_result HandleConfirmationPacket(tag_transport_packet packet)
        {
            tag_confirmation_packet confirmation_packet;

            if (e_convert_result.success != parser.ParseConfirmationPacket(packet, out confirmation_packet))
                return e_processing_result.failed;

            return e_processing_result.success;
        }

        private e_processing_result HandleTerminalStatePacket(tag_transport_packet packet)
        {
            tag_terminal_state_packet terminal_state_packet;

            if (e_convert_result.success != parser.ParseTerminalStatePacket(packet, out terminal_state_packet))
                return e_processing_result.failed;

            return e_processing_result.success;
        }

        private void RefreshCounters(string terminalName, Counters counters)
        {
            _hubClient.Invoke("RefreshCounters", terminalName, counters);
        }

        private void RefreshStatusBar(Guid groupId, TerminalLog terminalLog)
        {
            string extMessage = string.Format("{0} \"{1}\" : {2}", terminalLog.DateTimeTerminal.ToString(), Terminal.TerminalName, terminalLog.Message);
            _hubClient.Invoke("RefreshStatusBar", groupId, extMessage);
        }

        private void RefreshMessages(Guid groupId, string terminalName, TerminalLog terminalLog)
        {
            _hubClient.Invoke("RefreshMessages", groupId, terminalName, terminalLog);
        }

        private void HandleResult(tag_transport_packet packet, e_processing_result processing_result)
        {
            if (packet.type != e_packet_type.confirmation)
            {
                Confirmation(packet.type, processing_result);

                if (packet.type == e_packet_type.id &&
                    processing_result == e_processing_result.success)
                {
                    var settingsTerminal = SettingsTerminal.GetSettingsTerminalById(Terminal.Id);
                    if (null != settingsTerminal)
                    {
                        WriteSettings(settingsTerminal);
                    }
                }
            }
            else
            {
                if (processing_result == e_processing_result.success)
                {
                    switch (lastPacket)
                    {
                        case e_packet_type.settings:
                            var settingsTerminal = SettingsTerminal.GetSettingsTerminalById(Terminal.Id);
                            settingsTerminal.DataSent = true;
                            SettingsTerminal.Update(settingsTerminal);
                            break;
                        case e_packet_type.prize:
                            EventHandler handler = PrizeConfirmed;
                            if (handler != null)
                            {
                                handler(this, new EventArgs());
                            }
                            break;
                    }
                    lastPacket = e_packet_type.unknown;
                }
                else
                {
                    switch (lastPacket)
                    {
                        case e_packet_type.settings:
                            var settingsTerminal = SettingsTerminal.GetSettingsTerminalById(Terminal.Id);
                            WriteSettings(settingsTerminal);
                            break;
                        case e_packet_type.prize:
                            EventHandler handler = PrizeNoConfirmed;
                            if (handler != null)
                            {
                                handler(this, new EventArgs());
                            }
                            break;
                    }
                }
            }
        }
    }
}
