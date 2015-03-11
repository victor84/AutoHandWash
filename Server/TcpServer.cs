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
                byte[] bytes = new byte[256];
                while (running)
                {
                    TcpClient client = server.AcceptTcpClient();
                    NetworkStream stream = client.GetStream();
                    int i;
                    while ((i = stream.Read(bytes, 0, bytes.Length)) != 0)
                    {
                        // response = IParser.Response(bytes);
                        // data = IParser.Convert(bytes);
                        // IHub.Send(data);
                        stream.Write(bytes, 0, bytes.Length);
                    }
                    client.Close();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Main: {0}", e);
            }
        }
    }
}
