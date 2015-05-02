using Server.Caching;
using System;
using System.Collections.Generic;

namespace Server.Prize
{
    public class ServerPrizeCache
    {
        private Cache _cache;
        public ServerPrizeCache()
        {
            _cache = new Cache();
        }

        public IDictionary<Guid, GroupPrize> Load()
        {
            var val = _cache.Load(CacheKeys.PrizesGroups) as IDictionary<Guid, GroupPrize>;
            return val ?? new Dictionary<Guid, GroupPrize>();
        }

        public void Save(IDictionary<Guid, GroupPrize> value)
        {
            _cache.Save(CacheKeys.PrizesGroups, value);
        }
    }
}
