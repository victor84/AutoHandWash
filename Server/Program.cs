using System;
using Topshelf;

namespace Server
{
    class Program
    {
        private static void Main(string[] args)
        {
            AppSettings appSettings = AppSettings.Read();
            if (appSettings != null)
            {
                HostFactory.Run(hostConfigurator =>
                {
                    hostConfigurator.Service<Service>(serviceConfigurator =>
                    {
                        serviceConfigurator.ConstructUsing(() => new Service(appSettings));
                        serviceConfigurator.WhenStarted(service => service.Start());
                        serviceConfigurator.WhenStopped(service => service.Stop());
                    });
                    hostConfigurator.RunAsLocalSystem();
                    hostConfigurator.SetDisplayName("Сервис автомоек");
                    hostConfigurator.SetDescription("Сервис автомоек");
                    hostConfigurator.SetServiceName("AutoHandWash");
                });
            }
            else
            {
                throw new Exception("Проверьте наличие и правильность настроек приложения!");
            }
        }
    }
}
