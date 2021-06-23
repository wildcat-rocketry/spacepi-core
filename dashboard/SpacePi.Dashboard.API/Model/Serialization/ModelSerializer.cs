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
        public static void ToBase128(UInt64 number, Stream stream)
        {
            List<byte> bytes = new List<byte>();
            while (number > 0)
            {
                byte newValue = (byte)(number & 0x7F);

                number >>= 7;
                
                if(number > 0)
                {
                    newValue |= 0x80; 
                }

                bytes.Add(newValue);
            }

            stream.Write(bytes.ToArray(), 0, bytes.Count);
        }

        public static void ToBase128(Int64 number, Stream stream)
        {
            ToBase128((UInt64)number, stream); // Not perfect, negative numbers can get very long
            return;
        }

        public static UInt64 FromBase128(byte[] bytes, int maxLength = 10)
        {
            UInt64 newNumber = 0;
            for(int i = 0; i < maxLength; i++)
            {
                byte cur_byte = bytes[i];
                newNumber |= ((cur_byte & (UInt64)0x7F) << (i * 7));
                if((cur_byte & 0x80) == 0)
                {
                    break; // That was the last one
                }
            }

            return newNumber;
        }

        public static Int64 FromBase128Signed(byte[] bytes, int maxLength = 10)
        {
            return (Int64)FromBase128(bytes, maxLength);
        }

        public static void Serialize(this IObject obj, Stream stream) => throw new NotImplementedException();

        public static void Parse(this IObject obj, Stream stream) => throw new NotImplementedException();
    }
}
