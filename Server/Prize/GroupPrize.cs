using System;

namespace Server.Prize
{
    public class GroupPrize
    {
        public event EventHandler ValuePrizeReached;
        private uint valuePrize;
        private uint fund;

        public volatile bool WaitConfirm;

        public Guid GroupId { get; set; }

        public string GroupName { get; set; }

        public Guid LastTerminalId { get; set; }

        public uint Fund 
        {
            get
            {
                return fund;
            }
        }

        public byte Percent { get; set; }

        public uint ValuePrize
        {
            get 
            {
                return valuePrize;
            }
            set 
            {
                valuePrize = value;
            }
        }

        public void AddInFund(uint value)
        {
            fund += value;
            if (fund > valuePrize)
            {
                if (ValuePrizeReached != null)
                {
                    WaitConfirm = true;
                    ValuePrizeReached(this, new EventArgs());
                }
            }
        }
    }
}
