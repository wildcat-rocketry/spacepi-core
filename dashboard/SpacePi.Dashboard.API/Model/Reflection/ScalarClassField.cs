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

        public int Count { get; private set; } = 0;

        public int Number { get; }

        public bool IsTransient { get; }

        public IClass this[int idx] {
            get => Getter();
            set {
                if(idx >= Count || idx < 0) {
                    throw new InvalidOperationException("Invalid index");
                }
                Setter((Type)value);
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

        public ScalarClassField(string name, int number, bool transient, Func<Type> getter, Action<Type> setter) {
            Name = name;
            Number = number;
            IsTransient = transient;
            Getter = getter;
            Setter = setter;
            ChangedEvent = new(NotifyCollectionChangedAction.Replace, this, this, 0);
        }
    }
}
