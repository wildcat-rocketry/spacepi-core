using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Reflection {
    public class VectorEnumField<EnumType> : IEnumField where EnumType : Enum {
        private readonly ObservableCollection<EnumType> List;
        private readonly Func<EnumType, int> ToOrdinal;
        private readonly Func<int, EnumType> FromOrdinal;

        public string Name { get; }

        public bool IsList => true;

        public int Count => List.Count;

        public int this[int idx] {
            get => ToOrdinal(List[idx]);
            set => List[idx] = FromOrdinal(value);
        }

        public IEnum Type { get; }

        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public void Append() => List.Add(FromOrdinal(0));

        public void Remove(int idx) => List.RemoveAt(idx);

        private void Changed(object sender, NotifyCollectionChangedEventArgs e) {
            IEnumerable<int> Old = e.OldItems.Cast<EnumType>().Select(ToOrdinal);
            IEnumerable<int> New = e.NewItems.Cast<EnumType>().Select(ToOrdinal);
            switch (e.Action) {
                case NotifyCollectionChangedAction.Add:
                    e = new NotifyCollectionChangedEventArgs(e.Action, New.ToList(), e.NewStartingIndex);
                    break;
                case NotifyCollectionChangedAction.Move:
                    e = new NotifyCollectionChangedEventArgs(e.Action, New.ToList(), e.NewStartingIndex, e.OldStartingIndex);
                    break;
                case NotifyCollectionChangedAction.Remove:
                    e = new NotifyCollectionChangedEventArgs(e.Action, Old.ToList(), e.OldStartingIndex);
                    break;
                case NotifyCollectionChangedAction.Replace:
                    e = new NotifyCollectionChangedEventArgs(e.Action, New.ToList(), Old.ToList(), e.NewStartingIndex);
                    break;
                case NotifyCollectionChangedAction.Reset:
                default:
                    break;
            }
            CollectionChanged?.Invoke(this, e);
        }

        public VectorEnumField(string name, ObservableCollection<EnumType> list, Func<EnumType, int> toOrdinal, Func<int, EnumType> fromOrdinal) {
            Name = name;
            List = list;
            ToOrdinal = toOrdinal;
            FromOrdinal = fromOrdinal;
            List.CollectionChanged += Changed;
        }
    }
}
