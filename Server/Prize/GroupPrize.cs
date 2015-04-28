﻿using System;

namespace Server.Prize
{
    public class GroupPrize
    {
        public event EventHandler ValuePrizeReached;
        private ushort valuePrize;
        private uint fund;

        public volatile bool WaitConfirm;

        public Guid LastTerminalId { get; set; }

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
