using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// A dictionary which can be statically hashed and allocated as an array
    /// </summary>
    /// <typeparam name="TKey">The type of key</typeparam>
    /// <typeparam name="TValue">The type of value</typeparam>
    public class StaticDictionary<TKey, TValue> : IDictionary<TKey, TValue> {
        /// <summary>
        /// A collection of keys in the dictionary
        /// </summary>
        private class KeyCollection : ICollection<TKey> {
            /// <summary>
            /// The dictionary instance
            /// </summary>
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

            /// <summary>
            /// Creates a new KeyCollection
            /// </summary>
            /// <param name="dictionary">The dictionary instance</param>
            public KeyCollection(StaticDictionary<TKey, TValue> dictionary) => Dictionary = dictionary;
        }

        /// <summary>
        /// A collection of values in the dictionary
        /// </summary>
        private class ValueCollection : ICollection<TValue> {
            /// <summary>
            /// The dictionary instance
            /// </summary>
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

            /// <summary>
            /// Creates a new ValueCollection
            /// </summary>
            /// <param name="dictionary">The dictionary instance</param>
            public ValueCollection(StaticDictionary<TKey, TValue> dictionary) => Dictionary = dictionary;
        }

        /// <summary>
        /// The key comparer for the dictionary
        /// </summary>
        private readonly IEqualityComparer<TKey> KeyComparer;
        /// <summary>
        /// The static table of data for the dictionary
        /// </summary>
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

        /// <summary>
        /// Creates a statically-defined dictionary
        /// </summary>
        /// <param name="keyComparer">The key comparer</param>
        /// <param name="table">The static table of data</param>
        public StaticDictionary(IEqualityComparer<TKey> keyComparer, params (int Hash, TKey Key, TValue Value)[] table) {
            KeyComparer = keyComparer;
            Table = table;
            Keys = new KeyCollection(this);
            Values = new ValueCollection(this);
        }

        /// <summary>
        /// Hashes a set of data into a statically-defined dictionary
        /// </summary>
        /// <param name="keyComparer">The key comparer</param>
        /// <param name="source">The source to hash into the dictionary</param>
        public StaticDictionary(IEqualityComparer<TKey> keyComparer, IEnumerable<KeyValuePair<TKey, TValue>> source) :
            this(keyComparer, source.Select(k => (keyComparer.GetHashCode(k.Key), k.Key, k.Value)).OrderBy(t => t.Item1).ToArray()) {
        }
    }
}
