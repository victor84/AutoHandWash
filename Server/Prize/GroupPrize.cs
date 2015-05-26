using System;

namespace Server.Prize
{
    public class GroupPrize
    {
        public event EventHandler ValuePrizeReached;
        private ushort valuePrize;
        private double fund;

        public volatile bool WaitConfirm;

        public Guid GroupId { get; set; }

        public string GroupName { get; set; }

        public Guid LastTerminalId { get; set; }

        public double Fund 
        {
            get
            {
                return fund;
            }
        }

        public byte Percent { get; set; }

        public ushort ValuePrize
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

        public void AddInFund(double value)
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
