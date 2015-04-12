using Parsing;
using Server.Data;
using Server.Hubs;
using Server.Pipes;
using System;
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
                    terminal.OnCloseConnection += new EventHandler(RemoveTerminal);
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

        private void RemoveTerminal(TerminalHandler terminalHandler)
        {
            lock (lockList)
            {
                listTerminals.RemoveAll(x => x.Id == terminalHandler.Id);
            }
        }

        private void RemoveTerminal(object sender, EventArgs e)
        {
            RemoveTerminal((TerminalHandler)sender);
        }

        public void PipeMessageReceived(PipeMessage pipeMessage)
        {
            e_packet_type packetType = pipeMessage.PacketType;
            switch (packetType)
            { 
                case e_packet_type.settings:
                    SettingsTerminal settingsTerminal = (SettingsTerminal)pipeMessage.Message;
                    break;
            }
        }
    }
}