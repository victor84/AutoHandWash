using Server.Data;
using System;

namespace Server.Prize
{
    public class CountersArgs : EventArgs
    {
        public Counters Counters { get; set; }
        public CountersArgs(Counters counters)
        {
            Counters = counters;
        }
    }
}
