using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Reflection {
    public class ScalarClassField<Type> : IClassField where Type : IClass {
        private readonly Func<Type> Getter;
        private readonly Action<Type> Setter;

        public string Name { get; }

        public bool IsList => false;

        public int Count => 1;

        public int Number { get; }

        public bool IsTransient { get; }

        public IClass this[int idx] {
            get => Getter();
            set => throw new NotSupportedException();
        }

        public readonly NotifyCollectionChangedEventArgs ChangedEvent;

        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public void Append() => throw new InvalidOperationException();

        public void Remove(int idx) => throw new InvalidOperationException();

        public void Changed() => CollectionChanged?.Invoke(this, ChangedEvent);

        public ScalarClassField(string name, int number, bool transient, Func<Type> getter) {
            Name = name;
            Number = number;
            IsTransient = transient;
            Getter = getter;
            ChangedEvent = new(NotifyCollectionChangedAction.Replace, this, this, 0);
        }
    }
}
