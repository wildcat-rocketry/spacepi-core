using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel.Design;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;
using SpacePi.Dashboard.API.Model.Serialization;
using Xunit;

namespace SpacePi.Dashboard.Test {
    public class TestSerializeDeserialize {
        private class TestModelClass : ModelClass {
            public TestModelClass(string Name, IEnumerable<IField> fields) : base(Name) {
                Fields = fields;
            }
        }

        private static IEnumerable<T> RepeatList<T>(IEnumerable<T> to_repeat, int count) {
            for (int i = 0; i < count; i++) {
                foreach (T val in to_repeat) {
                    yield return val;
                }
            }
        }

        private static void VerifyStream(Stream stream, IEnumerable<byte> bytes) {
            Assert.Equal(bytes.Count(), stream.Length);

            foreach (byte b in bytes) {
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
        public void SimpleVarintDecode() {
            MemoryStream memStream = new MemoryStream(1);
            memStream.WriteByte(0b00000001);
            memStream.Seek(0, SeekOrigin.Begin);
            Assert.Equal(1, (long) Varint.FromBase128(memStream));
        }

        [Theory]
        [InlineData(new byte[] { 0b00000001 }, 1)]
        [InlineData(new byte[] { 0b10101100, 0b00000010 }, 300)]
        public void UnsignedVarintDecode(byte[] bytes, ulong num) {
            MemoryStream memStream = new MemoryStream(10);
            memStream.Write(bytes, 0, bytes.Length);
            memStream.Seek(0, SeekOrigin.Begin);
            Assert.Equal(num, Varint.FromBase128(memStream));
        }

        [Fact]
        public void NegativeVarintDecode() {
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
        public void NegativeVarint(long testVal) {
            MemoryStream memStream = new MemoryStream(12);
            Varint.ToBase128(testVal, memStream);
            memStream.Seek(0, SeekOrigin.Begin);
            Assert.Equal(testVal, Varint.FromBase128Signed(memStream));
        }

        public static IEnumerable<object[]> GetObjectSerializeData() {
            yield return new object[] {
                new TestObject(
                    new TestModelClass("Test1", new List<IField>{
                        new ScalarPrimitiveField("a", 1, false, IPrimitiveField.Types.Uint32, ()=>{ return (uint)150; }, (tmp)=>{ })
                    })
                ),
                new byte[] { 0x08, 0x96, 0x01 }
            };
            yield return new object[] {
                new TestObject(
                    new TestModelClass("Test1", new IField[] {
                        new ScalarClassField<ModelClass> ("name", 1, false, ()=>{
                                return new TestModelClass("NameMessage", new IField[] {
                                    new ScalarPrimitiveField("length", 1, false, IPrimitiveField.Types.Uint32, ()=>{ return (uint)1; }, (tmp)=>{ })
                                });
                            })
                    })
                ),
                new byte[] { 0x0a, 0x02, 0x08, 0x01 }
            };

            yield return new object[] {
                new TestObject(
                    new TestModelClass("RepeatedTest", new IField[] {
                        new ScalarClassField<ModelClass> ("data", 1, false, ()=>{
                                return new TestModelClass("DataMessage", new IField[] {
                                    new VectorPrimitiveField<uint>("data", 1, false, IPrimitiveField.Types.Uint32, new ObservableCollection<uint>(Enumerable.Repeat((uint)1, 100)))
                                });
                            })
                    })
                ),
                Enumerable.Concat(new byte[] { 0x0a, 0b11001000, 0b00000001}, RepeatList(new byte[] { 0x08, 0x01 }, 100))
            };

        }

        [Theory]
        [MemberData(nameof(GetObjectSerializeData))]
        public void SimpleObjectSerialize(TestObject testObject, IEnumerable<byte> bytes) {
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
        public void SimpleDataSerialize() {
            SimpleData data = new SimpleData(1, 2, "hi");
            MemoryStream stream = new MemoryStream(32);
            data.Serialize(stream);
            stream.Seek(0, SeekOrigin.Begin);
            VerifyStream(stream, new byte[] { 0x08, 0x01, 0x10, 0x02, 0x1a, 0x02, (byte) 'h', (byte) 'i' });
            stream.Close();
        }

        [Fact]
        public void SimpleDataDeserialize() {
            MemoryStream stream = new MemoryStream(new byte[] { 0x08, 0x01, 0x10, 0x02, 0x1a, 0x02, (byte) 'h', (byte) 'i' });
            SimpleData data = new SimpleData();
            data.Parse(stream);
            Assert.Equal(new SimpleData(1, 2, "hi"), data);
            stream.Close();
        }

        [Fact]
        public void SimpleDataListDeserialize() {
            MemoryStream stream = new MemoryStream(new byte[] { 0x0a, 0x08, 0x08, 0x01, 0x10, 0x02, 0x1a, 0x02, (byte) 'h', (byte) 'i' });
            SimpleDataList list = new();
            list.Parse(stream);
            Assert.Collection(list.List, new Action<SimpleData>[] { (data) => { Assert.Equal(new SimpleData(1, 2, "hi"), data); } });
        }

        [Fact]
        public void SimpleDataListDeserializeAgain() {
            MemoryStream stream = new MemoryStream(new byte[] { 0x0a, 0x08, 0x08, 0x04, 0x10, 0x01, 0x1a, 0x02, (byte) 'h', (byte) 'i', 0x0a, 0x09, 0x08, 0x03, 0x10, 0x02, 0x1a, 0x03, (byte) 'b', (byte) 'y', (byte) 'e' });
            SimpleDataList list = new();
            list.Parse(stream);
            Assert.Collection(list.List, new Action<SimpleData>[] {
                (data) => { Assert.Equal(new SimpleData(4, 1, "hi"), data); },
                (data) => { Assert.Equal(new SimpleData(3, 2, "bye"), data); },
            });
        }

        [Fact]
        public void SimpleDeserializeClass() {
            uint value = 0;
            TestObject test = new TestObject(
                new TestModelClass("RepeatedTest", new IField[] {
                    new ScalarClassField<ModelClass> ("data", 1, false, ()=>{
                            return new TestModelClass("DataMessage", new IField[] {
                                new ScalarPrimitiveField("value", 1, false, IPrimitiveField.Types.Uint32, () => {return 0; }, (val) => { value = (uint)val; } )
                            });
                        })
                })
            );

            byte[] bytes = new byte[] { 0x0a, 0x02, 0x08, 0x2c };
            MemoryStream stream = new MemoryStream(bytes);
            stream.SetLength(4);
            test.Parse(stream);
            Assert.Equal((uint) 0x2c, value);
        }

        [Fact]
        public void SimpleDeserializeList() {
            ObservableCollection<uint> values = new ObservableCollection<uint>();
            TestObject test = new TestObject(
                new TestModelClass("RepeatedTest", new IField[] {
                    new VectorPrimitiveField<uint>("values", 1, false, IPrimitiveField.Types.Uint32, values),
                })
            );

            byte[] bytes = new byte[] { 0x0a, 0x02, 0x08, 0x2c };
            MemoryStream stream = new MemoryStream(bytes);
            test.Parse(stream);
            Assert.Equal(2, values.Count);
            Assert.Equal((uint) 0x08, values[0]);
            Assert.Equal((uint) 0x2c, values[1]);
        }

        [Fact]
        public void DeserializeAnotherList() {
            ObservableCollection<uint> values = new ObservableCollection<uint>();
            TestObject test = new TestObject(
                new TestModelClass("RepeatedTest", new IField[] {
                    new VectorPrimitiveField<uint>("values", 1, false, IPrimitiveField.Types.Uint32, values),
                })
            );

            byte[] bytes = new byte[] { 0x0a, 0x03, 0x88, 0x01, 0x2c };
            MemoryStream stream = new MemoryStream(bytes);
            test.Parse(stream);
            Assert.Equal(2, values.Count);
            Assert.Equal((uint) 0x88, values[0]);
            Assert.Equal((uint) 0x2c, values[1]);
        }

        public class TestObject : IObject {
            public TestObject(ModelClass modelClass) {
                Reflection = modelClass;
            }

            public IClass Reflection { get; private set; }
        }

        public class SimpleDataList : IObject {
            public SimpleDataList() {
                Reflection = new SimpleDataListReflection(this);
            }

            public IClass Reflection { get; private set; }

            public List<SimpleData> List { get; set; } = new List<SimpleData>();
        }

        public class SimpleDataListReflection : IClass {
            public SimpleDataListReflection(SimpleDataList dataList) {
                Fields = new IField[] {
                    new VectorClassField<SimpleDataReflection>("List", 1, false, new ObservableCollection<SimpleDataReflection>(), () => {
                        SimpleData data = new SimpleData();
                        dataList.List.Add(data);
                        return (SimpleDataReflection)data.Reflection;
                    })
                };
            }

            public string Name => "SimpleDataList";

            public IEnumerable<IField> Fields { get; private set; }
        }

        public class SimpleData : IObject {
            public SimpleData() {
                Reflection = new SimpleDataReflection(this);
            }

            public SimpleData(uint a, uint b, string name) {
                A = a;
                B = b;
                Name = name;
                Reflection = new SimpleDataReflection(this);
            }

            public override bool Equals(object obj) {
                return obj is SimpleData data && data.A == A && data.B == B && data.Name == Name;
            }

            public uint A { get; set; }
            public uint B { get; set; }
            public string Name { get; set; }

            public IClass Reflection { get; private set; }
        }

        public class SimpleDataReflection : IClass {
            public SimpleDataReflection(SimpleData data) {
                Fields = new List<IField> {
                    new ScalarPrimitiveField("A", 1, false, IPrimitiveField.Types.Uint32, () => { return data.A; }, (a) => { data.A = (uint)a; }),
                    new ScalarPrimitiveField("B", 2, false, IPrimitiveField.Types.Uint32, () => { return data.B; }, (b) => { data.B = (uint)b; }),
                    new ScalarPrimitiveField("Name", 3, false, IPrimitiveField.Types.String, () => { return data.Name; }, (name) => { data.Name = (string)name; })
                };
            }

            public string Name { get; private set; } = "SimpleData";

            public IEnumerable<IField> Fields { get; private set; }
        }
    }
}
