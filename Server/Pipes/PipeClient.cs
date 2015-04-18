using System;
using System.IO;
using System.IO.Pipes;
using System.Runtime.Serialization.Formatters.Binary;

namespace Server.Pipes
{
    public class PipeClient
    {
        private NamedPipeClientStream pipeClient;
        public PipeClient()
        {
            pipeClient = new NamedPipeClientStream(PipeSettings.PipeName);
        }

        public void Write(ServerPacket serverPacket)
        {
            try 
            { 
                pipeClient.Connect();
                BinaryFormatter bf = new BinaryFormatter();
                using (MemoryStream ms = new MemoryStream())
                {
                    bf.Serialize(ms, serverPacket);
                    byte[] buffer = ms.ToArray();
                    pipeClient.Write(buffer, 0, buffer.Length);
                }
                pipeClient.Close();
            }
            catch (Exception e)
            {
                ServerLogger.Error(string.Format("PipeClient -> Write: {0}", e.Message));
            }
        }
    }
}
