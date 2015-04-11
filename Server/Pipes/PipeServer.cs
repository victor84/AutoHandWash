using System;
using System.IO;
using System.IO.Pipes;
using System.Runtime.Serialization.Formatters.Binary;
using System.Threading.Tasks;

namespace Server.Pipes
{
    public class PipeServer
    {
        private const int size = 2048;
        private NamedPipeServerStream pipeServer;
        private volatile bool running;
        private Task task;
        private Action<PipeMessage> method;
        public PipeServer(Action<PipeMessage> method)
        {
            this.pipeServer = new NamedPipeServerStream(PipeSettings.PipeName);
            this.method = method;
            this.task = new Task(() => Read());
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

        private void Read()
        {
            try
            {
                while (running)
                {
                    pipeServer.WaitForConnection();
                    byte[] buffer = new byte[size];
                    int countBytes = pipeServer.Read(buffer, 0, buffer.Length);
                    if (countBytes > 0)
                    {
                        BinaryFormatter formatter = new BinaryFormatter();
                        MemoryStream ms = new MemoryStream(buffer);
                        PipeMessage messageReceived = (PipeMessage)formatter.Deserialize(ms);
                        method(messageReceived);
                    }
                    pipeServer.Disconnect();
                }
                pipeServer.Close();
            }
            catch (Exception e)
            {
                Console.WriteLine("PipeServer -> Read: {0}", e);
            }
        }
    }
}
