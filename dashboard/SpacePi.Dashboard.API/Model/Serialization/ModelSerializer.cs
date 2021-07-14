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


        /// <summary>
        /// Serialize an IClass into the stream using the protobuf format.
        /// </summary>
        /// <param name="cls"></param>
        /// <param name="stream"></param>
        static void SerializeClass(IClass cls, Stream stream)
        {
            SerializeClass(cls, new WritingProtoStream(stream));
        }

        private static void SerializeClass(IClass cls, ProtoStream protostream)
        {
            foreach (IField field in cls.Fields) {
                if (field.IsTransient) continue;

                // Treat all lists as un-packed
                for (int i = 0; i < field.Count; i++)
                {
                    if (field is IEnumField enumField)
                    {
                        protostream.WriteFieldInfo(enumField.Number, WireType.VARINT);
                        Varint.ToBase128(enumField[i], protostream);
                    } else if (field is IPrimitiveField primitiveField)
                    {
                        switch (primitiveField.Type)
                        {
                            case IPrimitiveField.Types.Bool:
                                protostream.WriteFieldInfo(primitiveField.Number, WireType.VARINT);
                                protostream.WriteVarint(((bool)primitiveField[i])? (ulong)1: (ulong)0);
                                break;
                            case IPrimitiveField.Types.Int32:
                                protostream.WriteFieldInfo(primitiveField.Number, WireType.VARINT);
                                protostream.WriteSignedVarint((int)primitiveField[i]);
                                break;
                            case IPrimitiveField.Types.Int64:
                                protostream.WriteFieldInfo(primitiveField.Number, WireType.VARINT);
                                protostream.WriteSignedVarint((long)primitiveField[i]);
                                break;
                            case IPrimitiveField.Types.Uint32:
                                protostream.WriteFieldInfo(primitiveField.Number, WireType.VARINT);
                                protostream.WriteVarint((ulong)(uint)primitiveField[i]);
                                break;
                            case IPrimitiveField.Types.Uint64:
                                protostream.WriteFieldInfo(primitiveField.Number, WireType.VARINT);
                                protostream.WriteVarint((ulong)primitiveField[i]);
                                break;
                            case IPrimitiveField.Types.Double:
                                protostream.WriteFieldInfo(primitiveField.Number, WireType.FIXED_64);
                                byte[] doubleBytes = BitConverter.GetBytes((double)primitiveField[i]);
                                protostream.Write(doubleBytes, 0, 8);
                                break;
                            case IPrimitiveField.Types.Float:
                                protostream.WriteFieldInfo(primitiveField.Number, WireType.FIXED_32);
                                byte[] floatBytes = BitConverter.GetBytes((float)primitiveField[i]);
                                protostream.Write(floatBytes, 0, 4);
                                break;
                            case IPrimitiveField.Types.Bytes:
                                protostream.WriteFieldInfo(primitiveField.Number, WireType.LENGTH_DELIMITED);
                                byte[] bytes = (byte[])(primitiveField[i]);
                                protostream.WriteVarint((ulong)bytes.Length);
                                protostream.Write(bytes, 0, bytes.Length);
                                break;
                            case IPrimitiveField.Types.String:
                                protostream.WriteFieldInfo(primitiveField.Number, WireType.LENGTH_DELIMITED);
                                byte[] stringBytes = Encoding.UTF8.GetBytes((string)primitiveField[i]);
                                protostream.WriteVarint((ulong)stringBytes.Length);
                                protostream.Write(stringBytes, 0, stringBytes.Length);
                                break;
                            case IPrimitiveField.Types.External:
                                throw new NotSupportedException("External type must always be transient");
                        }
                    } else if (field is IClassField classField)
                    {
                        using CountingProtoStream counting = new();
                        SerializeClass(classField[i], counting);
                        protostream.WriteFieldInfo(classField.Number, WireType.LENGTH_DELIMITED);
                        protostream.WriteVarint((ulong)counting.Length);
                        SerializeClass(classField[i], protostream);
                    }
                }
            }
        }
    }
}
