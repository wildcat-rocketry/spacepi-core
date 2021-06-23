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
            using(MemoryStream memStream = new MemoryStream(4))
            {
                ModelSerializer.ToBase128(1, memStream);
                Assert.Equal(1, memStream.Position);
                memStream.Position = 0;
                int firstByte = memStream.ReadByte();
                Assert.True(firstByte == 0x01);
            }
        }

        [Fact]
        public void SlightlyMoreComplexVarintEncode() {
            using(MemoryStream memStream = new MemoryStream(4))
            {
                ModelSerializer.ToBase128(300, memStream);
                Assert.Equal(2, memStream.Position);
                memStream.Position = 0;
                Assert.Equal(0b10101100, memStream.ReadByte());
                Assert.Equal(0b00000010, memStream.ReadByte());
            }
        }

        [Fact]
        public void MuchMoreComplexVarintEncode() {
            using(MemoryStream memStream = new MemoryStream(8))
            {
                ModelSerializer.ToBase128(12345678, memStream);
                Assert.Equal(4, memStream.Position);
                memStream.Position = 0;
                Assert.Equal(0b11001110, memStream.ReadByte());
                Assert.Equal(0b11000010, memStream.ReadByte());
                Assert.Equal(0b11110001, memStream.ReadByte());
                Assert.Equal(0b00000101, memStream.ReadByte());
            }
        }

        [Fact]
        public void NegativeVarintEncode() {
            using(MemoryStream memStream = new MemoryStream(12))
            {
                ModelSerializer.ToBase128(-3345, memStream);
                Assert.Equal(10, memStream.Position);
                memStream.Position = 0;
                Assert.Equal(0b11101111, memStream.ReadByte());
                Assert.Equal(0b11100101, memStream.ReadByte());
                Assert.Equal(0b11111111, memStream.ReadByte());
                Assert.Equal(0b11111111, memStream.ReadByte());
                Assert.Equal(0b11111111, memStream.ReadByte());
                Assert.Equal(0b11111111, memStream.ReadByte());
                Assert.Equal(0b11111111, memStream.ReadByte());
                Assert.Equal(0b11111111, memStream.ReadByte());
                Assert.Equal(0b11111111, memStream.ReadByte());
                Assert.Equal(0b00000001, memStream.ReadByte());
            }
        }

        [Fact]
        public void SimpleVarintDecode()
        {
            byte[] bytes = { 0b00000001 };
            Assert.Equal(1, (long)ModelSerializer.FromBase128(bytes, 1));
        }

        [Fact]
        public void SlightlyMoreComplexVarintDecode()
        {
            byte[] bytes = { 0b10101100, 0b00000010 };
            Assert.Equal(300, (long)ModelSerializer.FromBase128(bytes, 2));
        }

        [Fact]
        public void NegativeVarintDecode()
        {
            byte[] bytes = { 0b11101111, 0b11100101, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00000001 };
            Assert.Equal(-3345, ModelSerializer.FromBase128Signed(bytes, 10));
        }


    }
}
