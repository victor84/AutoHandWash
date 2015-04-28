using System;
using System.Configuration;
namespace Server
{
    public class AppSettings
    {
        public string BaseUri { get; set; }
        public int Port { get; set; }

        public static AppSettings Read()
        {
            AppSettings appSettings = null;
            try
            {
                string baseUri = ConfigurationManager.AppSettings["BaseUri"];
                string portString = ConfigurationManager.AppSettings["Port"];
                int port = int.Parse(portString);
                if (!(port >= 1 && port <= 65535))
                    throw new FormatException("Порт вне диапазона от 1 до 65535");
                appSettings = new AppSettings() 
                { 
                    BaseUri = baseUri,
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
