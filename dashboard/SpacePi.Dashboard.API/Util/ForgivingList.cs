using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Util {
    public class ForgivingList<Type> : IList<Type> {
        private readonly IList<Type> Base;

        public Type this[int index] {
            get {
                if (index < 0 || index >= Base.Count) {
                    return default;
                }
                return Base[index];
            }
            set {
                if (index >= 0 || index < Base.Count) {
                    Base[index] = value;
                }
            }
        }

        public int Count => Base.Count;

        public bool IsReadOnly => Base.IsReadOnly;

        public void Add(Type item) => Base.Add(item);

        public void Clear() => Base.Clear();

        public bool Contains(Type item) => Base.Contains(item);

        public void CopyTo(Type[] array, int arrayIndex) => Base.CopyTo(array, arrayIndex);

        public IEnumerator<Type> GetEnumerator() => Base.GetEnumerator();

        public int IndexOf(Type item) => Base.IndexOf(item);

        public void Insert(int index, Type item) {
            if (index < 0) {
                index = 0;
            }
            if (index > Base.Count) {
                index = Base.Count;
            }
            Base.Insert(index, item);
        }

        public bool Remove(Type item) => Base.Remove(item);

        public void RemoveAt(int index) {
            if (index >= 0 && index < Base.Count) {
                Base.RemoveAt(index);
            }
        }

        IEnumerator IEnumerable.GetEnumerator() => Base.GetEnumerator();

        public ForgivingList(IList<Type> list) => Base = list;
    }
}
