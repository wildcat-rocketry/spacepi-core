using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Serialization
{
    public class GrowingStream : Stream
    {
        private const long _buffer_length = 64;
        private long _array_i = 0;
        private long _byte_i = 0;
        private long _byte_count = 0;

        private List<byte[]> data = new List<byte[]> { new byte[_buffer_length] };

        public override bool CanRead => true;

        public override bool CanSeek => true;

        public override bool CanWrite => true;

        public override long Length => (data.Count - 1) * _buffer_length + _byte_count;

        public override long Position { 
            get => _array_i * _buffer_length + _byte_i;
            set {
                if (value < 0) Position = 0;

                if(value > Length)
                {
                    long _num_array = (value / _buffer_length) + 1;
                    for(long i = data.Count; i < _num_array; i++)
                    {
                        data.Add(new byte[64]);
                    }
                    _byte_count = value - (_num_array - 1) * _buffer_length;
                }

                _array_i = value / _buffer_length;
                _byte_i = value - _array_i * _buffer_length;
            }
        }

        public override void Flush()
        {
            return;
        }

        public override long Seek(long offset, SeekOrigin origin)
        {
            switch (origin)
            {
                case SeekOrigin.Begin:
                    Position = offset;
                    break;
                case SeekOrigin.Current:
                    Position = offset + Position;
                    break;
                case SeekOrigin.End:
                    Position = offset + Length;
                    break;
            }

            return Position;
        }

        public override void SetLength(long value)
        {
            return; // Ignore set length
        }

        public override void WriteByte(byte value)
        {
            data[(int)_array_i][(int)_byte_i] = value;
            Position++;
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
            for(int i = offset; i < offset + count; i++)
            {
                WriteByte(buffer[i]);
            }
        }

        public override int ReadByte()
        {
            if (Position == Length) return -1;

            byte value = data[(int)_array_i][(int)_byte_i];
            Position++;
            return value;
        }

        public override int Read(byte[] buffer, int offset, int count)
        {
            int written = 0;
            int read;
            for(int i = offset; i < offset + count; i++)
            {
                read = ReadByte();
                if (read < 0) return written;
                else
                {
                    buffer[i] = (byte)read;
                    written++;
                }
            }

            return written;
        }
    }
}
