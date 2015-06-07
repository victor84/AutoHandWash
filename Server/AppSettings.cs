using System;
using System.Configuration;
namespace Server
{
    public class AppSettings
    {
        public ushort HostPort { get; set; }
        public ushort Port { get; set; }

        public static AppSettings Read()
        {
            AppSettings appSettings = null;
            try
            {
                string hostPortString = ConfigurationManager.AppSettings["HostPort"];
                ushort hostPort = ushort.Parse(hostPortString);
                if (!(hostPort >= 1 && hostPort <= 65535))
                    throw new FormatException("Порт вне диапазона от 1 до 65535");

                string portString = ConfigurationManager.AppSettings["Port"];
                ushort port = ushort.Parse(portString);
                if (!(port >= 1 && port <= 65535))
                    throw new FormatException("Порт вне диапазона от 1 до 65535");

                appSettings = new AppSettings() 
                { 
                    HostPort = hostPort,
                    Port = port
                };
            }
            catch (Exception e)
            {
                ServerLogger.Error(string.Format("AppSettings -> Read -> {0}", e.Message));
            }
            return appSettings;
        }
    }
}
