using Server.Data;
using System;

namespace Server.Prize
{
    public class InputArgs : EventArgs
    {
        public uint Input { get; set; }
        public InputArgs(uint input)
        {
            Input = input;
        }
    }
}
