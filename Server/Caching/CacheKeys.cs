using System;

namespace Server.Caching
{
    public class CacheKeys
    {
        public static readonly CacheKeys PrizesGroups = new CacheKeys(1, "PrizesGroups");
        
        private readonly string name;
        private readonly int value;

        private CacheKeys(int value, string name)
        {
            this.name = name;
            this.value = value;
        }

        public override string ToString()
        {
            return name;
        }
    }
}
