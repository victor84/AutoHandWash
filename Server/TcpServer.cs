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

        public TcpServer(int port)
        {
            task = new Task(() => Main(port));
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

        private void Main(int port)
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
                    ClientHandler handler = new ClientHandler(client);
                    handler.Run();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("TcpServer -> Main: {0}", e);
            }
        }
    }
}
