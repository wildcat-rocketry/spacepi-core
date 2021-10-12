using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Serialization {
    public abstract class ProtoStream : Stream {
        public abstract override bool CanRead { get; }

        public abstract override bool CanSeek { get; }

        public abstract override bool CanWrite { get; }

        public abstract override long Length { get; }

        public abstract void WriteFieldInfo(int number, WireType type);

        public abstract void WriteVarint(ulong number);

        public abstract void WriteSignedVarint(long number);

        public abstract override long Position { get; set; }

        public abstract override void Flush();

        public abstract override int Read(byte[] buffer, int offset, int count);

        public abstract override long Seek(long offset, SeekOrigin origin);

        public abstract override void SetLength(long value);

        public abstract override void Write(byte[] buffer, int offset, int count);
    }
}
