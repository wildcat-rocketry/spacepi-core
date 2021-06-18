using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Util;

namespace SpacePi.Dashboard.API.Model.Reflection {
    public class VectorPrimitiveField<PrimType> : IPrimitiveField {
        private readonly IList<PrimType> List;

        public string Name { get; }

        public bool IsList => true;

        public int Count => List.Count;

        public int Number { get; }

        public IPrimitiveField.Types Type { get; }

        public object this[int idx] {
            get => List[idx];
            set => List[idx] = (PrimType) value;
        }

        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public void Append() => List.Add(default);

        public void Remove(int idx) => List.RemoveAt(idx);

        private void Changed(object sender, NotifyCollectionChangedEventArgs e) => CollectionChanged?.Invoke(this, e);

        public VectorPrimitiveField(string name, int number, IPrimitiveField.Types type, ObservableCollection<PrimType> list) {
            Name = name;
            Number = number;
            Type = type;
            List = new ForgivingList<PrimType>(list);
            list.CollectionChanged += Changed;
        }
    }
}
