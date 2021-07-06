using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.API.Model.Serialization {
    public static class ModelSerializer {
        /// <summary>
        /// Stream a UInt64 into the stream
        /// </summary>
        /// <param name="number"></param>
        public static void ToBase128(ulong number, Stream stream)
        {
            while (number > 0)
            {
                byte newValue = (byte)(number & 0x7F);

                number >>= 7;
                
                if(number > 0)
                {
                    newValue |= 0x80; 
                }

                stream.WriteByte(newValue);
            }
        }

        public static void ToBase128(long number, Stream stream)
        {
            ToBase128((ulong)((number<<1) ^ (number>>63)), stream); // Not perfect, negative numbers can get very long
            return;
        }

        public static ulong FromBase128(Stream stream)
        {
            ulong newNumber = 0;
            
            for(int i = 0; i < 10; i++) // The max size of a varint is 10
            {
                int int_stream_return = stream.ReadByte();
                if (int_stream_return < 1) break; // gracefully hit end of stream

                byte cur_byte = (byte)int_stream_return;

                newNumber |= ((cur_byte & (ulong)0x7F) << (i * 7));
                if((cur_byte & 0x80) == 0)
                {
                    break; // That was the last one
                }
            }

            return newNumber;
        }

        public static long FromBase128Signed(Stream stream)
        {
            long num = (long)FromBase128(stream);
            if((num & 0x01) != 0)
            {
                num ^= -1;
            }
            return num >> 1;
        }

        public static void Serialize(this IObject obj, Stream stream) => throw new NotImplementedException();

        public static void Parse(this IObject obj, Stream stream) => throw new NotImplementedException();
    }
}
