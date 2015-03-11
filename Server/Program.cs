using System;
using Topshelf;

namespace Server
{
    class Program
    {
        private static ServiceInfo serviceInfo = new ServiceInfo() 
        { 
            Url = "http://localhost:8080",
            HubName = "testHub",
            Port = 13000
        };
        
        private static void Main(string[] args)
        {
            HostFactory.Run(hostConfigurator =>
            {
                hostConfigurator.Service<Service>(serviceConfigurator =>
                {
                    serviceConfigurator.ConstructUsing(() => new Service(serviceInfo));
                    serviceConfigurator.WhenStarted(service => service.Start());
                    serviceConfigurator.WhenStopped(service => service.Stop());
                });
                hostConfigurator.RunAsLocalSystem();
                hostConfigurator.SetDisplayName("Сервис автомоек");
                hostConfigurator.SetDescription("Сервис автомоек");
                hostConfigurator.SetServiceName("AutoHandWash");
            });
        }
    }
}
