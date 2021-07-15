using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    public class StaticDictionary<TKey, TValue> : IDictionary<TKey, TValue> {
        private class KeyCollection : ICollection<TKey> {
            private readonly StaticDictionary<TKey, TValue> Dictionary;

            public bool IsReadOnly => true;

            public int Count => Dictionary.Count;

            public void CopyTo(TKey[] array, int arrayIndex) {
                if (arrayIndex + Dictionary.Count >= array.Length) {
                    throw new ArgumentException("Destination array is not large enough");
                }
                foreach ((int _, TKey key, TValue _) in Dictionary.Table) {
                    array[arrayIndex++] = key;
                }
            }

            public bool Contains(TKey item) => Dictionary.ContainsKey(item);

            public IEnumerator<TKey> GetEnumerator() => Dictionary.Table.Select(t => t.Key).GetEnumerator();

            IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

            public void Add(TKey item) => throw new NotSupportedException();
            public void Clear() => throw new NotSupportedException();
            public bool Remove(TKey item) => throw new NotSupportedException();

            public KeyCollection(StaticDictionary<TKey, TValue> dictionary) => Dictionary = dictionary;
        }

        private class ValueCollection : ICollection<TValue> {
            private readonly StaticDictionary<TKey, TValue> Dictionary;

            public bool IsReadOnly => true;

            public int Count => Dictionary.Count;

            public void CopyTo(TValue[] array, int arrayIndex) {
                if (arrayIndex + Dictionary.Count >= array.Length) {
                    throw new ArgumentException("Destination array is not large enough");
                }
                foreach ((int _, TKey _, TValue val) in Dictionary.Table) {
                    array[arrayIndex++] = val;
                }
            }

            public bool Contains(TValue item) => Dictionary.Table.Any(t => item.Equals(t.Value));

            public IEnumerator<TValue> GetEnumerator() => Dictionary.Table.Select(t => t.Value).GetEnumerator();

            IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

            public void Add(TValue item) => throw new NotSupportedException();
            public void Clear() => throw new NotSupportedException();
            public bool Remove(TValue item) => throw new NotSupportedException();

            public ValueCollection(StaticDictionary<TKey, TValue> dictionary) => Dictionary = dictionary;
        }

        private readonly IEqualityComparer<TKey> KeyComparer;
        public readonly (int Hash, TKey Key, TValue Value)[] Table;

        public bool IsReadOnly => true;

        public int Count => Table.Length;

        public ICollection<TKey> Keys { get; }

        public ICollection<TValue> Values { get; }

        public void CopyTo(KeyValuePair<TKey, TValue>[] array, int arrayIndex) {
            if (arrayIndex + Table.Length >= array.Length) {
                throw new ArgumentException("Destination array is not large enough");
            }
            foreach ((int _, TKey key, TValue val) in Table) {
                array[arrayIndex++] = new KeyValuePair<TKey, TValue>(key, val);
            }
        }

        public bool TryGetValue(TKey key, out TValue value) {
            int hash = KeyComparer.GetHashCode(key);
            int min = 0;
            int max = Table.Length - 1;
            while (min <= max) {
                int mid = (min + max) / 2;
                if (Table[mid].Hash < hash) {
                    min = mid + 1;
                } else if (Table[mid].Hash > hash) {
                    max = mid - 1;
                } else {
                    value = Table[mid].Value;
                    return true;
                }
            }
            value = default;
            return false;
        }

        public TValue this[TKey key] {
            get {
                if (!TryGetValue(key, out TValue val)) {
                    throw new KeyNotFoundException();
                }
                return val;
            }
            set => throw new NotSupportedException();
        }

        public IEnumerator<KeyValuePair<TKey, TValue>> GetEnumerator() => Table.Select(t => new KeyValuePair<TKey, TValue>(t.Key, t.Value)).GetEnumerator();

        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

        public bool Contains(KeyValuePair<TKey, TValue> item) {
            if (TryGetValue(item.Key, out TValue val)) {
                return item.Value.Equals(val);
            }
            return false;
        }

        public bool ContainsKey(TKey key) => TryGetValue(key, out TValue _);

        public void Add(TKey key, TValue value) => throw new NotSupportedException();
        public void Add(KeyValuePair<TKey, TValue> item) => throw new NotSupportedException();
        public void Clear() => throw new NotSupportedException();
        public bool Remove(TKey key) => throw new NotSupportedException();
        public bool Remove(KeyValuePair<TKey, TValue> item) => throw new NotSupportedException();

        public StaticDictionary(IEqualityComparer<TKey> keyComparer, params (int Hash, TKey Key, TValue Value)[] table) {
            KeyComparer = keyComparer;
            Table = table;
            Keys = new KeyCollection(this);
            Values = new ValueCollection(this);
        }

        public StaticDictionary(IEqualityComparer<TKey> keyComparer, IEnumerable<KeyValuePair<TKey, TValue>> source) :
            this(keyComparer, source.Select(k => (keyComparer.GetHashCode(k.Key), k.Key, k.Value)).OrderBy(t => t.Item1).ToArray()) {
        }
    }
}
