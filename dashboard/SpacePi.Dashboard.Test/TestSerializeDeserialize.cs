using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using Xunit;

using SpacePi.Dashboard.API.Model.Serialization;
using SpacePi.Dashboard.API.Model.Reflection;
using System.Collections.ObjectModel;
using System.ComponentModel.Design;

namespace SpacePi.Dashboard.Test {
    public class TestSerializeDeserialize {
        private static IEnumerable<T> RepeatList<T>(IEnumerable<T> to_repeat, int count)
        {
            for(int i = 0; i < count; i++)
            {
                foreach (T val in to_repeat)
                {
                    yield return val;
                }
            }
        }

        private static void VerifyStream(Stream stream, IEnumerable<byte> bytes)
        {
            Assert.Equal(bytes.Count(), stream.Length);

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

            yield return new object[] {
                new TestObject(
                    new ModelClass("RepeatedTest", new IField[] {
                        new ScalarClassField<ModelClass> ("data", 1, false, ()=>{
                                return new ModelClass("DataMessage", new IField[] {
                                    new VectorPrimitiveField<uint>("data", 1, false, IPrimitiveField.Types.Uint32, new ObservableCollection<uint>(Enumerable.Repeat((uint)1, 100)))
                                });
                            }, (tmp)=>{ })
                    })
                ),
                Enumerable.Concat(new byte[] { 0x0a, 0b11001000, 0b00000001}, RepeatList(new byte[] { 0x08, 0x01 }, 100))
            };

        }

        [Theory]
        [MemberData(nameof(GetObjectSerializeData))]
        public void SimpleObjectSerialize(TestObject testObject, IEnumerable<byte> bytes)
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

        [Fact]
        public void SimpleDeserializeClass() {
            uint value = 0;
            TestObject test = new TestObject(
                new ModelClass("RepeatedTest", new IField[] {
                    new ScalarClassField<ModelClass> ("data", 1, false, ()=>{
                            return new ModelClass("DataMessage", new IField[] {
                                new ScalarPrimitiveField("value", 1, false, IPrimitiveField.Types.Uint32, () => {return 0; }, (val) => { value = (uint)val; } )
                            });
                        }, (tmp)=>{ })
                })
            );

            byte[] bytes = new byte[] { 0x0a, 0x02, 0x08, 0x2c };
            MemoryStream stream = new MemoryStream(bytes);
            stream.SetLength(4);
            test.Parse(stream);
            Assert.Equal((uint)0x2c, value);
        }

        [Fact]
        public void SimpleDeserializeList() {
            ObservableCollection<uint> values = new ObservableCollection<uint>();
            TestObject test = new TestObject(
                new ModelClass("RepeatedTest", new IField[] {
                    new VectorPrimitiveField<uint>("values", 1, false, IPrimitiveField.Types.Uint32, values),
                })
            );

            byte[] bytes = new byte[] { 0x0a, 0x02, 0x08, 0x2c };
            MemoryStream stream = new MemoryStream(bytes);
            test.Parse(stream);
            Assert.Equal(2, values.Count);
            Assert.Equal((uint)0x08, values[0]);
            Assert.Equal((uint)0x2c, values[1]);
        }

        [Fact]
        public void DeserializeAnotherList() {
            ObservableCollection<uint> values = new ObservableCollection<uint>();
            TestObject test = new TestObject(
                new ModelClass("RepeatedTest", new IField[] {
                    new VectorPrimitiveField<uint>("values", 1, false, IPrimitiveField.Types.Uint32, values),
                })
            );

            byte[] bytes = new byte[] { 0x0a, 0x03, 0x88, 0x01, 0x2c };
            MemoryStream stream = new MemoryStream(bytes);
            test.Parse(stream);
            Assert.Equal(2, values.Count);
            Assert.Equal((uint)0x88, values[0]);
            Assert.Equal((uint)0x2c, values[1]);

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
