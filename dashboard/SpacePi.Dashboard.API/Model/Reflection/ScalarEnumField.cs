using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Reflection {
    public class ScalarEnumField : IEnumField {
        private readonly Func<int> Getter;
        private readonly Action<int> Setter;

        public string Name { get; }

        public bool IsList => false;

        public int Count => 1;

        public int Number { get; }

        public IEnum Type { get; }

        public int this[int idx] {
            get => Getter();
            set => Setter(value);
        }

        public readonly NotifyCollectionChangedEventArgs ChangedEvent;

        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public void Append() => throw new InvalidOperationException();

        public void Remove(int idx) => throw new InvalidOperationException();

        public void Changed() => CollectionChanged?.Invoke(this, ChangedEvent);

        public ScalarEnumField(string name, int number, IEnum type, Func<int> getter, Action<int> setter) {
            Name = name;
            Number = number;
            Type = type;
            Getter = getter;
            Setter = setter;
            ChangedEvent = new(NotifyCollectionChangedAction.Replace, this, this, 0);
        }
    }
}
