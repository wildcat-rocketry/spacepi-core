﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
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
        public static void Serialize(this IObject obj, Stream stream) {
            var cls = obj.Reflection;
            SerializeClass(cls, stream);
        }

        public static void Parse(this IObject obj, Stream stream) => ParseClass(obj.Reflection, stream);

        public static Tuple<int, WireType> ReadFieldInfo(Stream stream) {
            ulong num = Varint.FromBase128(stream);
            return new Tuple<int, WireType>((int) (num >> 3), (WireType) (num & 0x7));
        }


        static void DiscardIfNot(Stream stream, WireType wireType, WireType actual, Action func){
            if (wireType == actual) {
                func();
            } else {
                DiscardWire(actual, stream);
            }
        }

        static object ParseSingleVarint(IPrimitiveField.Types type, Stream stream) {
            switch (type) {
                case IPrimitiveField.Types.Bool:
                    return Varint.FromBase128(stream) != 0;
                case IPrimitiveField.Types.Int32:
                    return (int)Varint.FromBase128Signed(stream);
                case IPrimitiveField.Types.Int64:
                    return (long)Varint.FromBase128Signed(stream);
                case IPrimitiveField.Types.Uint32:
                    return (uint)Varint.FromBase128(stream);
                case IPrimitiveField.Types.Uint64:
                    return (ulong)Varint.FromBase128(stream);
                default:
                    throw new InvalidOperationException("This function can only parse values from varints");
            }
        }

        static void ParseClass(IClass cls, Stream stream) {
            // How do I want to deal with invalid stuff?

            bool done = false;
            while (stream.Position != stream.Length && !done) {
                Tuple<int, WireType> fieldInfo = ReadFieldInfo(stream);
                WireType wireType = fieldInfo.Item2;
                int fieldNum = fieldInfo.Item1;

                IField field = cls.Fields.Where((field) => { return field.Number == fieldNum; }).SingleOrDefault();
                if( field != null ) {
                    if (field is ScalarEnumField enumField) {
                        DiscardIfNot(stream, WireType.VARINT, wireType, () => {
                            enumField[0] = (int) Varint.FromBase128(stream);
                        });
                    } else if (field is ScalarPrimitiveField primField) {
                        IPrimitiveField.Types fieldType = primField.Type;
                        switch (fieldType) {
                            case IPrimitiveField.Types.Bool:
                            case IPrimitiveField.Types.Int32:
                            case IPrimitiveField.Types.Int64:
                            case IPrimitiveField.Types.Uint32:
                            case IPrimitiveField.Types.Uint64:
                                DiscardIfNot(stream, WireType.VARINT, wireType, () => {
                                    primField[0] = ParseSingleVarint(fieldType, stream);
                                });
                                break;
                            case IPrimitiveField.Types.Double:
                                DiscardIfNot(stream, WireType.FIXED_64, wireType, () => {
                                    primField[0] = new BinaryReader(stream).ReadDouble();
                                });
                                break;
                            case IPrimitiveField.Types.Float:
                                DiscardIfNot(stream, WireType.FIXED_32, wireType, () => {
                                    primField[0] = new BinaryReader(stream).ReadSingle();
                                });
                                break;
                            case IPrimitiveField.Types.String:
                                DiscardIfNot(stream, WireType.LENGTH_DELIMITED, wireType, () => {
                                    ulong length = Varint.FromBase128(stream);
                                    StringWriter stringWriter = new();
                                    for (ulong i = 0; i < length; i++) {
                                        stringWriter.Write(stream.ReadByte());
                                    }
                                    primField[0] = stringWriter.ToString();
                                });
                                break;
                        }
                    } else if (field is ScalarClassField<ModelClass> scalarClassField) {
                        IClass model = scalarClassField[0];
                        DiscardIfNot(stream, WireType.LENGTH_DELIMITED, wireType, () => {
                            ulong length = Varint.FromBase128(stream);
                            ParseClass(model, stream);
                        });
                    } else if (field is VectorEnumField<Enum> vectorEnum) {
                        Action addOne = () => {
                            vectorEnum.Append();
                            vectorEnum[vectorEnum.Count - 1] = (int) Varint.FromBase128(stream);
                        };
                        if (wireType == WireType.LENGTH_DELIMITED) {
                            ulong length = Varint.FromBase128(stream);
                            for (ulong i = 0; i < length; i++) {
                                addOne();
                            }
                        } else {
                            DiscardIfNot(stream, WireType.VARINT, wireType, () => {
                                addOne();
                            });
                        }
                    } else if (field is VectorPrimitiveField<object> vectorPrim) {
                        IPrimitiveField.Types fieldType = vectorPrim.Type;
                        switch (fieldType) {
                            case IPrimitiveField.Types.Bool:
                            case IPrimitiveField.Types.Int32:
                            case IPrimitiveField.Types.Int64:
                            case IPrimitiveField.Types.Uint32:
                            case IPrimitiveField.Types.Uint64:
                                Action addOne = () => {
                                    vectorPrim.Append();
                                    vectorPrim[vectorPrim.Count - 1] = ParseSingleVarint(fieldType, stream);
                                };
                                if (wireType == WireType.VARINT) {
                                    ulong length = Varint.FromBase128(stream);
                                    for (ulong i = 0; i < length; i++) {
                                        addOne();
                                    }
                                } else {
                                    DiscardIfNot(stream, WireType.VARINT, wireType, () => {
                                        addOne();
                                    });
                                }
                                break;
                            case IPrimitiveField.Types.Double:
                                DiscardIfNot(stream, WireType.FIXED_64, wireType, () => {
                                    vectorPrim.Append();
                                    vectorPrim[vectorPrim.Count - 1] = new BinaryReader(stream).ReadDouble();
                                });
                                break;
                            case IPrimitiveField.Types.Float:
                                DiscardIfNot(stream, WireType.FIXED_32, wireType, () => {
                                    vectorPrim.Append();
                                    vectorPrim[vectorPrim.Count - 1] = new BinaryReader(stream).ReadSingle();
                                });
                                break;
                            case IPrimitiveField.Types.String:
                                DiscardIfNot(stream, WireType.LENGTH_DELIMITED, wireType, () => {
                                    ulong length = Varint.FromBase128(stream);
                                    StringWriter stringWriter = new();
                                    for (ulong i = 0; i < length; i++) {
                                        stringWriter.Write(stream.ReadByte());
                                    }
                                    vectorPrim.Append();
                                    vectorPrim[vectorPrim.Count - 1] = stringWriter.ToString();
                                });
                                break;
                        }
                    } else if (field is VectorClassField<IClass> vectorClass) {
                        DiscardIfNot(stream, WireType.LENGTH_DELIMITED, wireType, () => {
                            vectorClass.Append();
                            ParseClass(vectorClass[vectorClass.Count - 1], stream);
                        });
                    }
                }
            }
        }

        static void DiscardWire(WireType type, Stream stream) {
            switch (type) {
                case WireType.VARINT:
                    Varint.FromBase128(stream);
                    break;
                case WireType.FIXED_64:
                    stream.Seek(8, SeekOrigin.Current);
                    break;
                case WireType.LENGTH_DELIMITED:
                    ulong length = Varint.FromBase128(stream);
                    stream.Seek((long)length, SeekOrigin.Current);
                    break;
                case WireType.FIXED_32:
                    stream.Seek(4, SeekOrigin.Current);
                    break;
                default:
                    break;
            }
        }

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
