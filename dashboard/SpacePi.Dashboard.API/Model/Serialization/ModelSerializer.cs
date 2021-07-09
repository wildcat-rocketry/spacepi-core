using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.API.Model.Serialization {
    public static class ModelSerializer {
        enum WireType
        {
            VARINT = 0,
            FIXED_64 = 1,
            LENGTH_DELIMITED = 2,
            START_GROUP = 3,
            END_GROUP = 4,
            FIXED_32 = 5,
        }

        static void AddFieldInfo(this Stream stream, int field_number, WireType type)
        {
            Varint.ToBase128((ulong)((field_number << 3) | (int)type), stream);
        }

        static void SerializeClass(IClass cls, Stream stream)
        {
            foreach (IField field in cls.Fields) {
                if (field.IsTransient) continue;

                // Treat all lists as un-packed
                for (int i = 0; i < field.Count; i++)
                {
                    if (field is IEnumField enumField)
                    {
                        stream.AddFieldInfo(enumField.Number, WireType.VARINT);
                        Varint.ToBase128(enumField[i], stream);
                    } else if (field is IPrimitiveField primitiveField)
                    {
                        switch (primitiveField.Type)
                        {
                            case IPrimitiveField.Types.Bool:
                                stream.AddFieldInfo(primitiveField.Number, WireType.VARINT);
                                Varint.ToBase128(((bool)primitiveField[i])? (ulong)1: (ulong)0, stream);
                                break;
                            case IPrimitiveField.Types.Int32:
                                stream.AddFieldInfo(primitiveField.Number, WireType.VARINT);
                                Varint.ToBase128((int)primitiveField[i], stream);
                                break;
                            case IPrimitiveField.Types.Int64:
                                stream.AddFieldInfo(primitiveField.Number, WireType.VARINT);
                                Varint.ToBase128((long)primitiveField[i], stream);
                                break;
                            case IPrimitiveField.Types.Uint32:
                                stream.AddFieldInfo(primitiveField.Number, WireType.VARINT);
                                Varint.ToBase128((ulong)(uint)primitiveField[i], stream);
                                break;
                            case IPrimitiveField.Types.Uint64:
                                stream.AddFieldInfo(primitiveField.Number, WireType.VARINT);
                                Varint.ToBase128((ulong)primitiveField[i], stream);
                                break;
                            case IPrimitiveField.Types.Double:
                                stream.AddFieldInfo(primitiveField.Number, WireType.FIXED_64);
                                byte[] doubleBytes = BitConverter.GetBytes((double)primitiveField[i]);
                                stream.Write(doubleBytes, 0, 8);
                                break;
                            case IPrimitiveField.Types.Float:
                                stream.AddFieldInfo(primitiveField.Number, WireType.FIXED_32);
                                byte[] floatBytes = BitConverter.GetBytes((float)primitiveField[i]);
                                stream.Write(floatBytes, 0, 4);
                                break;
                            case IPrimitiveField.Types.Bytes:
                                stream.AddFieldInfo(primitiveField.Number, WireType.LENGTH_DELIMITED);
                                byte[] bytes = (byte[])(primitiveField[i]);
                                Varint.ToBase128(bytes.Length, stream);
                                stream.Write(bytes, 0, bytes.Length);
                                break;
                            case IPrimitiveField.Types.String:
                                stream.AddFieldInfo(primitiveField.Number, WireType.LENGTH_DELIMITED);
                                byte[] stringBytes = Encoding.UTF8.GetBytes((string)primitiveField[i]);
                                Varint.ToBase128(stringBytes.Length, stream);
                                stream.Write(stringBytes, 0, stringBytes.Length);
                                break;
                            case IPrimitiveField.Types.External:
                                throw new NotImplementedException("Unknown external type");
                        }
                    } else if (field is IClassField classField)
                    {
                        GrowingStream growingStream = new();
                        SerializeClass(classField[i], growingStream);
                        stream.AddFieldInfo(classField.Number, WireType.LENGTH_DELIMITED);
                        Varint.ToBase128((ulong)growingStream.Length, stream);
                        growingStream.Position = 0;
                        growingStream.CopyTo(stream);
                        growingStream.Close();
                    }
                }
            }
        }
        
        /// <summary>
        /// This will need to take the IObject then get the reflection IClass. Then look through the fields of the IClass and Serialize
        /// </summary>
        /// <param name="obj">The object to serialize</param>
        /// <param name="stream">The space to serialize into</param>
        public static void Serialize(this IObject obj, Stream stream)
        {
            var cls = obj.Reflection;
            SerializeClass(cls, stream);
        }

        public static void Parse(this IObject obj, Stream stream) => throw new NotImplementedException();
    }

    class GrowingStream : Stream
    {
        private const long _buffer_length = 64;
        private long _array_i = 0;
        private long _byte_i = 0;
        private long _byte_count = 0;

        private List<byte[]> data = new List<byte[]> { new byte[_buffer_length] };

        public override bool CanRead => true;

        public override bool CanSeek => true;

        public override bool CanWrite => true;

        public override long Length => (data.Count - 1) * _buffer_length + _byte_count;

        public override long Position { 
            get => _array_i * _buffer_length + _byte_i;
            set {
                if (value < 0) Position = 0;

                if(value > Length)
                {
                    long _num_array = (value / _buffer_length) + 1;
                    for(long i = data.Count; i < _num_array; i++)
                    {
                        data.Add(new byte[64]);
                    }
                    _byte_count = value - (_num_array - 1) * _buffer_length;
                }

                _array_i = value / _buffer_length;
                _byte_i = value - _array_i * _buffer_length;
            }
        }

        public override void Flush()
        {
            return;
        }

        public override long Seek(long offset, SeekOrigin origin)
        {
            switch (origin)
            {
                case SeekOrigin.Begin:
                    Position = offset;
                    break;
                case SeekOrigin.Current:
                    Position = offset + Position;
                    break;
                case SeekOrigin.End:
                    Position = offset + Length;
                    break;
            }

            return Position;
        }

        public override void SetLength(long value)
        {
            return; // Ignore set length
        }

        public override void WriteByte(byte value)
        {
            data[(int)_array_i][(int)_byte_i] = value;
            Position++;
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
            for(int i = offset; i < offset + count; i++)
            {
                WriteByte(buffer[i]);
            }
        }

        public override int ReadByte()
        {
            if (Position == Length) return -1;

            byte value = data[(int)_array_i][(int)_byte_i];
            Position++;
            return value;
        }

        public override int Read(byte[] buffer, int offset, int count)
        {
            int written = 0;
            int read;
            for(int i = offset; i < offset + count; i++)
            {
                read = ReadByte();
                if (read < 0) return written;
                else
                {
                    buffer[i] = (byte)read;
                    written++;
                }
            }

            return written;
        }
    }
}
