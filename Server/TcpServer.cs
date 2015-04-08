using Server.Hubs;
using System;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;

namespace Server
{
    public class TcpServer
    {
        private string ipString = "127.0.0.1";
        private volatile bool running;
        private Task task;

        public TcpServer(int port, IHubClient hubClient)
        {
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
                    TerminalHandler hubTerminals = new TerminalHandler(client, hubClient);
                    hubTerminals.Run();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("TcpServer -> Main: {0}", e);
            }
        }
    }
}
