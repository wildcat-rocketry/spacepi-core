using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Serialization
{
    public static class Varint
    {
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

                if (number > 0)
                {
                    newValue |= 0x80;
                }

                stream.WriteByte(newValue);
            }
        }

        private static ulong SignedConversion(long number)
        {
            return (ulong)((number << 1) ^ (number >> 63));
        }

        public static void ToBase128(long number, Stream stream)
        {
            ToBase128(SignedConversion(number), stream); // Not perfect, negative numbers can get very long
            return;
        }

        public static ulong FromBase128(Stream stream, ulong max_len = 10) => FromBase128(stream, max_len, out _);

        public static ulong FromBase128(Stream stream, ulong max_len, out ulong length)
        {
            ulong newNumber = 0;

            for (length = 0; length < max_len && length < 10; length++) // The max size of a varint is 10
            {
                int int_stream_return = stream.ReadByte();
                if (int_stream_return < 1) { throw new EndOfStreamException(); }; // gracefully hit end of stream

                byte cur_byte = (byte)int_stream_return;

                newNumber |= ((cur_byte & (ulong)0x7F) << ((int)length * 7));
                if ((cur_byte & 0x80) == 0)
                {
                    break; // That was the last one
                }
            }

            length++;
            return newNumber;
        }

        public static long FromBase128Signed(Stream stream, ulong max_len = 10) => FromBase128Signed(stream, max_len, out _);

        public static long FromBase128Signed(Stream stream, ulong max_len, out ulong length)
        {
            long num = (long)FromBase128(stream, max_len, out length);
            if ((num & 0x01) != 0)
            {
                num ^= -1;
            }
            return num >> 1;
        }

        public static int Base128Length(long number)
        {
            return Base128Length(SignedConversion(number));
        }

        private static ulong ByteThreashold(int i)
        {
            return (ulong)1 << (i * 7);
        }

        public static int Base128Length(ulong number)
        {
            if (number >= ByteThreashold(5))
            {
                if (number >= ByteThreashold(7))
                {
                    if(number >= ByteThreashold(8))
                    {
                        if(number >= ByteThreashold(9))
                        {
                            return 10;
                        } else
                        {
                            return 9;
                        }
                    } else
                    {
                        return 8;
                    }
                } else
                {
                    if(number >= ByteThreashold(6))
                    {
                        return 7;
                    } else
                    {
                        return 6;
                    }
                }
            } else
            {
                if (number >= ByteThreashold(3))
                {
                    if (number >= ByteThreashold(4))
                    {
                        return 5;
                    }
                    else
                    {
                        return 4;
                    }
                }
                else if (number >= ByteThreashold(2))
                {
                    return 3;
                } else if(number >= ByteThreashold(1))
                {
                    return 2;
                } else
                {
                    return 1;
                }
            }
        }
    }
}
