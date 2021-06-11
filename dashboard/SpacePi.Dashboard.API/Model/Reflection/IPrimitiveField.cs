using System;
using System.Buffers.Text;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Reflection {
    public interface IPrimitiveField : IField {
        public delegate bool TryParser(string str, out object val);

        private static bool Copy(bool ret, object @in, out object @out) {
            @out = @in;
            return ret;
        }

        public enum Types : int {
            Double,
            Float,
            Int32,
            Int64,
            Uint32,
            Uint64,
            Bool,
            String,
            Bytes,
            External,
        }

        public static readonly Func<object, string>[] ToStringers = new Func<object, string>[] {
            o => o.ToString(),
            o => o.ToString(),
            o => o.ToString(),
            o => o.ToString(),
            o => o.ToString(),
            o => o.ToString(),
            o => o.ToString(),
            o => o.ToString(),
            o => BitConverter.ToString((byte[]) o),
            o => $"{o.GetType().FullName}@{o.GetHashCode():X8}",
        };

        public static readonly TryParser[] TryParsers = new TryParser[] {
            (string str, out object val) => Copy(double.TryParse(str, out double v), v, out val),
            (string str, out object val) => Copy(float.TryParse(str, out float v), v, out val),
            (string str, out object val) => Copy(int.TryParse(str, out int v), v, out val),
            (string str, out object val) => Copy(long.TryParse(str, out long v), v, out val),
            (string str, out object val) => Copy(uint.TryParse(str, out uint v), v, out val),
            (string str, out object val) => Copy(ulong.TryParse(str, out ulong v), v, out val),
            (string str, out object val) => Copy(bool.TryParse(str, out bool v), v, out val),
            (string str, out object val) => Copy(true, str, out val),
            (string str, out object val) => Copy(false, null, out val),
            (string str, out object val) => Copy(false, null, out val),
        };

        public Types Type { get; }

        object this[int i] { get; set; }
    }
}
