using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Parsing
{
    public static class Extensions
    {
        public static Byte[] SubArray(this Byte[] data, UInt32 offset, UInt32 count)
        {
            if (0 == count)
                return null;

            Byte[] result = new Byte[count];

            Array.Copy(data, offset, result, 0, count);

            return result;
        }

        public static Char[] ToCharArray(this Byte[] data)
        {
            if (null == data)
                return null;

            if (0 == data.Length)
                return new Char[0];

            Char[] result = new Char[data.Length];

            for (int i = 0; i < data.Length; ++i)
            {
                result[i] = (Char)data[i];
            }

            return result;
        }

        public static Byte[] ToByteArray(this Char[] data)
        {
            if (null == data)
                return null;

            if (0 == data.Length)
                return new Byte[0];

            Byte[] result = new Byte[data.Length];

            for (int i = 0; i < data.Length; ++i)
            {
                result[i] = (Byte)data[i];
            }

            return result;
        }


    }
}
