using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using Xunit;

using SpacePi.Dashboard.API.Model.Serialization;

namespace SpacePi.Dashboard.Test {
    public class TestSerializeDeserialize {
        [Fact]
        public void SimpleVarintEncode() {
            MemoryStream memStream = new MemoryStream(4);
            ModelSerializer.ToBase128((ulong)1, memStream);
            Assert.Equal(1, memStream.Position);
            memStream.Position = 0;
            int firstByte = memStream.ReadByte();
            Assert.True(firstByte == 0x01);
        }

        [Fact]
        public void SlightlyMoreComplexVarintEncode() {
            MemoryStream memStream = new MemoryStream(4);
            ModelSerializer.ToBase128((ulong)300, memStream);
            Assert.Equal(2, memStream.Position);
            memStream.Position = 0;
            Assert.Equal(0b10101100, memStream.ReadByte());
            Assert.Equal(0b00000010, memStream.ReadByte());
        }

        [Fact]
        public void MuchMoreComplexVarintEncode() {
            MemoryStream memStream = new MemoryStream(8);
            ModelSerializer.ToBase128((ulong)12345678, memStream);
            Assert.Equal(4, memStream.Position);
            memStream.Position = 0;
            Assert.Equal(0b11001110, memStream.ReadByte());
            Assert.Equal(0b11000010, memStream.ReadByte());
            Assert.Equal(0b11110001, memStream.ReadByte());
            Assert.Equal(0b00000101, memStream.ReadByte());
        }

        [Fact]
        public void SimpleVarintDecode()
        {
            MemoryStream memStream = new MemoryStream(1);
            memStream.WriteByte(0b00000001);
            memStream.Seek(0, SeekOrigin.Begin);
            Assert.Equal(1, (long)ModelSerializer.FromBase128(memStream));
        }

        [Fact]
        public void SlightlyMoreComplexVarintDecode()
        {
            MemoryStream memStream = new MemoryStream(2);
            memStream.WriteByte(0b10101100);
            memStream.WriteByte(0b00000010);
            memStream.Seek(0, SeekOrigin.Begin);
            Assert.Equal(300, (long)ModelSerializer.FromBase128(memStream));
        }

        [Fact]
        public void NegativeVarintDecode()
        {
            MemoryStream memStream = new MemoryStream(10);
            memStream.WriteByte(0b00000001);
            memStream.Seek(0, SeekOrigin.Begin);
            Assert.Equal(-1, ModelSerializer.FromBase128Signed(memStream));
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
            ModelSerializer.ToBase128(testVal, memStream);
            memStream.Seek(0, SeekOrigin.Begin);
            Assert.Equal(testVal, ModelSerializer.FromBase128Signed(memStream));
        }
    }
}
