using System.Collections.Generic;
using System.Runtime.Caching;

namespace Server.Caching
{
    public class Cache
    {
        private static object _lockCache = new object();
        private static ObjectCache _cache = MemoryCache.Default;

        public object Load(CacheKeys key)
        {
            lock (_lockCache)
            {
                return _cache[key.ToString()] as object;   
            }
        }

        public void Save(CacheKeys key, object value)
        {
            lock (_lockCache)
            {
                var policy = new CacheItemPolicy();
                _cache.Set(key.ToString(), value, policy);
            }
        }
    }
}
