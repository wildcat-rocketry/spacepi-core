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

        static void AddFieldInfo(this Stream stream, int field_number, WireType type)
        {
            Varint.ToBase128((ulong)((field_number << 3) | (int)type), stream);
        }

        /// <summary>
        /// Serialize an IClass into the stream using the protobuf format.
        /// </summary>
        /// <param name="cls"></param>
        /// <param name="stream"></param>
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
    }
}
