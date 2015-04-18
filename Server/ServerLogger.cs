using NLog;
using System;
namespace Server
{
    public class ServerLogger
    {
        private static Logger logger = LogManager.GetCurrentClassLogger();

        public static void Info(string message)
        {
            logger.Info(DateTime.Now + " > " + message);
        }

        public static void Error(string message)
        {
            logger.Error(DateTime.Now + " > " + message);
        }
    }
}
