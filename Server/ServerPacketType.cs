namespace Server
{
    public enum ServerPacketType
    {
        settingsTerminal, // настройки терминала
        settingsGroup,    // настойки группы
        fillcache,	      // пополнить баланс
        changeStatus,	  // изменить статус
        prize,            // приз
    };
}
