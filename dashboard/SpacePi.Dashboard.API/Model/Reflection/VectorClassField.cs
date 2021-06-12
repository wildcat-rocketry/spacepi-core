using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Reflection {
    public class VectorClassField<Type> : IClassField where Type : IClass {
        private readonly ObservableCollection<Type> List;
        private readonly Func<Type> Ctor;

        public string Name { get; }

        public bool IsList => true;

        public int Count => List.Count;

        public IClass this[int idx] {
            get => List[idx];
            set => List[idx] = (Type) value;
        }

        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public void Append() => List.Add(Ctor());

        public void Remove(int idx) => List.RemoveAt(idx);

        private void Changed(object sender, NotifyCollectionChangedEventArgs e) => CollectionChanged?.Invoke(this, e);

        public VectorClassField(string name, ObservableCollection<Type> list, Func<Type> ctor) {
            Name = name;
            List = list;
            Ctor = ctor;
            List.CollectionChanged += Changed;
        }
    }
}
