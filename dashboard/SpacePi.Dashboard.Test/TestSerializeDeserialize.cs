using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using Xunit;

using SpacePi.Dashboard.API.Model.Serialization;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Test {
    public class TestSerializeDeserialize {
        private void VerifyStream(Stream stream, byte[] bytes)
        {
            foreach (byte b in bytes)
            {
                Assert.Equal(b, stream.ReadByte());
            }
        }

        [Theory]
        [InlineData(1, new byte[] { 0x01 })]
        [InlineData(300, new byte[] { 0b10101100, 0b00000010 })]
        [InlineData(12345678, new byte[] { 0b11001110, 0b11000010, 0b11110001, 0b00000101 })]

        public void UnsignedVarintEncode(ulong num, byte[] bytes) {
            MemoryStream memStream = new MemoryStream(10);
            Varint.ToBase128(num, memStream);
            Assert.Equal(bytes.Length, memStream.Position);
            memStream.Position = 0;
            VerifyStream(memStream, bytes);
            memStream.Close();
        }

        [Fact]
        public void SimpleVarintDecode()
        {
            MemoryStream memStream = new MemoryStream(1);
            memStream.WriteByte(0b00000001);
            memStream.Seek(0, SeekOrigin.Begin);
            Assert.Equal(1, (long)Varint.FromBase128(memStream));
        }

        [Theory]
        [InlineData(new byte[] { 0b00000001 }, 1)]
        [InlineData(new byte[] { 0b10101100, 0b00000010 }, 300)]
        public void UnsignedVarintDecode(byte[] bytes, ulong num)
        {
            MemoryStream memStream = new MemoryStream(10);
            memStream.Write(bytes, 0, bytes.Length);
            memStream.Seek(0, SeekOrigin.Begin);
            Assert.Equal(num, Varint.FromBase128(memStream));
        }

        [Fact]
        public void NegativeVarintDecode()
        {
            MemoryStream memStream = new MemoryStream(10);
            memStream.WriteByte(0b00000001);
            memStream.Seek(0, SeekOrigin.Begin);
            Assert.Equal(-1, Varint.FromBase128Signed(memStream));
        }

        [Theory]
        [InlineData(-420)]
        [InlineData(420)]
        [InlineData(69)]
        [InlineData(-69)]
        [InlineData(-6942069)]
        public void NegativeVarint(long testVal)
        {
            MemoryStream memStream = new MemoryStream(12);
            Varint.ToBase128(testVal, memStream);
            memStream.Seek(0, SeekOrigin.Begin);
            Assert.Equal(testVal, Varint.FromBase128Signed(memStream));
        }

        public static IEnumerable<object[]> GetObjectSerializeData()
        {
            yield return new object[] {
                new TestObject(
                    new ModelClass("Test1", new IField[] {
                        new ScalarPrimitiveField("a", 1, false, IPrimitiveField.Types.Uint32, ()=>{ return (uint)150; }, (tmp)=>{ })
                    })
                ),
                new byte[] { 0x08, 0x96, 0x01 }
            };
            yield return new object[] {
                new TestObject(
                    new ModelClass("Test1", new IField[] {
                        new ScalarClassField<ModelClass> ("name", 1, false, ()=>{
                                return new ModelClass("NameMessage", new IField[] {
                                    new ScalarPrimitiveField("length", 1, false, IPrimitiveField.Types.Uint32, ()=>{ return (uint)1; }, (tmp)=>{ })
                                });
                            }, (tmp)=>{ })
                    })
                ),
                new byte[] { 0x0a, 0x02, 0x08, 0x01 }
            };

        }

        [Theory]
        [MemberData(nameof(GetObjectSerializeData))]
        public void SimpleObjectSerialize(TestObject testObject, byte[] bytes)
        {
            /* Representation of:
             *     message Test1 {
             *         int32 a = 1; // Value of 150
             *     }
             */
            MemoryStream stream = new MemoryStream(32);
            testObject.Serialize(stream);
            stream.Seek(0, SeekOrigin.Begin);
            VerifyStream(stream, bytes);
            stream.Close();
        }

        public class TestObject : IObject
        {
            public TestObject(ModelClass modelClass){
                Reflection = modelClass;
            }

            public IClass Reflection { get; private set; }
        }
    }
}
