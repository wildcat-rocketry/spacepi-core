using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Reflection {
    public class ScalarPrimitiveField : IPrimitiveField {
        private readonly Func<object> Getter;
        private readonly Action<object> Setter;

        public string Name { get; }

        public bool IsList => false;

        public int Count => 1;

        public int Number { get; }

        public IPrimitiveField.Types Type { get; }

        public object this[int idx] {
            get => Getter();
            set => Setter(value);
        }

        public readonly NotifyCollectionChangedEventArgs ChangedEvent;

        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public void Append() => throw new InvalidOperationException();

        public void Remove(int idx) => throw new InvalidOperationException();

        public void Changed() => CollectionChanged?.Invoke(this, ChangedEvent);

        public ScalarPrimitiveField(string name, int number, IPrimitiveField.Types type, Func<object> getter, Action<object> setter) {
            Name = name;
            Number = number;
            Type = type;
            Getter = getter;
            Setter = setter;
        }
    }
}
