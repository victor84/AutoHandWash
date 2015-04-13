using Server.Hubs;
using System;
using System.Linq;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;

namespace Server
{
    public class TcpServer
    {
        private object lockList;
        private string ipString = "127.0.0.1";
        private volatile bool running;
        private Task task;
        private List<TerminalHandler> listTerminals; 

        public TcpServer(int port, IHubClient hubClient)
        {
            lockList = new object();
            listTerminals = new List<TerminalHandler>();
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
                    terminal.CloseConnection += new EventHandler(OnRemoveTerminal);
                    terminal.Run();
                    AddTerminal(terminal);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("TcpServer -> Main: {0}", e);
            }
        }

        private void AddTerminal(TerminalHandler terminalHandler)
        {
            lock (lockList)
            {
                listTerminals.Add(terminalHandler);
            }
        }

        private TerminalHandler GetTerminal(Guid terminalId)
        {
            TerminalHandler terminalHandler = null;
            lock (lockList)
            {
                terminalHandler = listTerminals.Where(x => x.Terminal != null && x.Terminal.Id == terminalId).FirstOrDefault();
            }
            return terminalHandler;
        }

        private void RemoveTerminal(TerminalHandler terminalHandler)
        {
            lock (lockList)
            {
                listTerminals.RemoveAll(x => x.Id == terminalHandler.Id);
            }
        }

        private void OnRemoveTerminal(object sender, EventArgs e)
        {
            RemoveTerminal((TerminalHandler)sender);
        }

        public void ServerPacketReceived(ServerPacket serverPacket)
        {
            var terminal = GetTerminal(serverPacket.TerminalId);
            if (terminal != null)
            {
                terminal.EnqueueServerPacket(serverPacket);
            }
        }
    }
}