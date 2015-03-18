using Microsoft.Owin.Hosting;
using System;

namespace Server
{
    public class Service
    {
        private IDisposable webApp;
        private TcpServer tcpServer;
        private AppSettings appSettings;
        
        public Service(AppSettings appSettings)
        {
            this.appSettings = appSettings;
            tcpServer = new TcpServer(appSettings.Port);
        }

        public void Start()
        {
            tcpServer.Start();
            webApp = WebApp.Start<Startup>(appSettings.BaseUri);
        }

        public void Stop()
        {
            tcpServer.Stop();
            webApp.Dispose();
        }
    }
}
