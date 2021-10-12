using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Serialization {
    public class CountingProtoStream : ProtoStream {
        public CountingProtoStream() {
        }

        public override bool CanRead => false;

        public override bool CanSeek => true;

        public override bool CanWrite => true;

        private long _length;
        public override long Length => _length;

        public override void WriteFieldInfo(int number, WireType type) {
            Position += Varint.Base128Length((ulong) ((number << 3) | (int) type));
        }

        public override void WriteVarint(ulong number) {
            Position += Varint.Base128Length(number);
        }

        public override void WriteSignedVarint(long number) {
            Position += Varint.Base128Length(number);
        }

        private long _position = 1;
        public override long Position {
            get => _position;
            set {
                if (value <= 0)
                    return;

                if (value > _length + 1) {
                    _length = value - 1;
                }
                _position = value;
            }
        }

        public override void Flush() {
        }

        public override int Read(byte[] buffer, int offset, int count) {
            return 0;
        }

        public override long Seek(long offset, SeekOrigin origin) {
            switch (origin) {
                case SeekOrigin.Begin:
                    Position = offset;
                    break;
                case SeekOrigin.End:
                    Position = Length + offset + 1;
                    break;
                case SeekOrigin.Current:
                    Position += offset;
                    break;
            }

            return Position;
        }

        public override void SetLength(long value) {
            _length = value;
            if (Position > value) {
                Position = value;
            }
        }

        public override void Write(byte[] buffer, int offset, int count) {
            Position += count;
        }
    }
}
