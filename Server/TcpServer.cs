using Server.Hubs;
using System;
using System.Linq;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;
using Server.Prize;
using Server.Data;

namespace Server
{
    public class TcpServer
    {
        private object lockList;
        private string ipString = "127.0.0.1";
        private volatile bool running;
        private Task task;
        private List<TerminalHandler> listTerminalHandlers;
        private ServerPrize serverPrize;
       
        public TcpServer(int port, IHubClient hubClient)
        {
            lockList = new object();
            listTerminalHandlers = new List<TerminalHandler>();
            serverPrize = new ServerPrize();
            task = new Task(() => Main(port, hubClient));
        }

        public void Start()
        {
            running = true;
            task.Start();
        }

        public void Stop()
        {
            running = false;
        }

        private void Main(int port, IHubClient hubClient)
        {
            TcpListener server = null;
            try
            {
                IPAddress localAddr = IPAddress.Parse(ipString);
                server = new TcpListener(localAddr, port);
                server.Start();
                while (running)
                {
                    TcpClient client = server.AcceptTcpClient();
                    TerminalHandler terminal = new TerminalHandler(client, hubClient);
                    terminal.ConnectionClosed += new EventHandler(OnRemoveTerminal);
                    terminal.CommonInputExtended += new EventHandler<InputArgs>(OnCommonInputExtended);
                    terminal.PrizeConfirmed += new EventHandler(OnPrizeConfirmed);
                    terminal.PrizeNoConfirmed += new EventHandler(OnPrizeNoConfirmed);
                    terminal.Run();
                    AddTerminalHandler(terminal);
                }
            }
            catch (Exception e)
            {
                ServerLogger.Error(string.Format("TcpServer -> Main: {0}", e.Message));
            }
        }

        private void AddTerminalHandler(TerminalHandler terminalHandler)
        {
            lock (lockList)
            {
                listTerminalHandlers.Add(terminalHandler);
            }
        }

        private TerminalHandler GetTerminalHandler(Guid terminalId)
        {
            TerminalHandler terminalHandler = null;
            lock (lockList)
            {
                terminalHandler = listTerminalHandlers.Where(x => x.Terminal != null && x.Terminal.Id == terminalId).FirstOrDefault();
            }
            return terminalHandler;
        }

        private void RemoveTerminalHandler(TerminalHandler terminalHandler)
        {
            lock (lockList)
            {
                listTerminalHandlers.RemoveAll(x => x.Id == terminalHandler.Id);
            }
        }

        private void OnRemoveTerminal(object sender, EventArgs e)
        {
            RemoveTerminalHandler((TerminalHandler)sender);
        }

        private void OnCommonInputExtended(object sender, InputArgs e)
        {
            var terminalHandler = (TerminalHandler)sender;
            var terminal = terminalHandler.Terminal;
            if (terminal != null)
            {
                serverPrize.CommonInputExtended(terminal, e.Input);
            }
        }

        private void OnPrizeNoConfirmed(object sender, EventArgs e)
        {
            var terminalHandler = (TerminalHandler)sender;
            var terminal = terminalHandler.Terminal;
            if (terminal != null)
            {
                serverPrize.PrizeNotConfirmed(terminal);
            }
        }

        private void OnPrizeConfirmed(object sender, EventArgs e)
        {
            var terminalHandler = (TerminalHandler)sender;
            var terminal = terminalHandler.Terminal;
            if (terminal != null)
            {
                serverPrize.PrizeConfirmed(terminal.GroupId);
            }
        }

        public void ServerPacketReceived(ServerPacket serverPacket)
        {
            if (serverPacket.PacketType == ServerPacketType.settingsGroup)
            {
                serverPrize.ChangeSettingsGroup((SettingsGroup)serverPacket.Data);
            }
            else
            {
                var terminal = GetTerminalHandler(serverPacket.Id);
                if (terminal != null)
                {
                    terminal.EnqueueServerPacket(serverPacket);
                }
            }
        }
    }
}