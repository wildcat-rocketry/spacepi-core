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

        public int Count { get; private set; } = 0;

        public int Number { get; }

        public bool IsTransient { get; }

        public IPrimitiveField.Types Type { get; }

        public object this[int idx] {
            get => Getter();
            set {
                if(idx >= Count || idx < 0) {
                    throw new InvalidOperationException("Invalid index");
                }
                Setter(value);
            }
        }

        public readonly NotifyCollectionChangedEventArgs ChangedEvent;

        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public void Append() {
            Count++; 
        }

        public void Remove(int idx) {
            if (Count > 0) {
                Count--;
            } else {
                throw new InvalidOperationException("No value to remove");
            }
        }

        public void Changed() => CollectionChanged?.Invoke(this, ChangedEvent);

        public ScalarPrimitiveField(string name, int number, bool transient, IPrimitiveField.Types type, Func<object> getter, Action<object> setter) {
            Name = name;
            Number = number;
            IsTransient = transient;
            Type = type;
            Getter = getter;
            Setter = setter;
        }
    }
}
