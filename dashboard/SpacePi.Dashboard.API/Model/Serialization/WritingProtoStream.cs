using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Serialization {
    public class WritingProtoStream : ProtoStream {
        private Stream _stream;

        public WritingProtoStream(Stream stream) {
            _stream = stream;
        }

        public override bool CanRead => _stream.CanRead;

        public override bool CanSeek => _stream.CanSeek;

        public override bool CanWrite => _stream.CanWrite;

        public override long Length => _stream.Length;

        public override void WriteFieldInfo(int number, WireType type) {
            WriteVarint((ulong) ((number << 3) | (int) type));
        }

        public override void WriteVarint(ulong number) {
            Varint.ToBase128(number, _stream);
        }

        public override void WriteSignedVarint(long number) {
            Varint.ToBase128(number, _stream);
        }

        public override long Position {
            get => _stream.Position;
            set { _stream.Position = value; }
        }

        public override void Flush() {
            _stream.Flush();
        }

        public override int Read(byte[] buffer, int offset, int count) {
            return _stream.Read(buffer, offset, count);
        }

        public override long Seek(long offset, SeekOrigin origin) {
            return _stream.Seek(offset, origin);
        }

        public override void SetLength(long value) {
            _stream.SetLength(value);
        }

        public override void Write(byte[] buffer, int offset, int count) {
            _stream.Write(buffer, offset, count);
        }
    }
}
