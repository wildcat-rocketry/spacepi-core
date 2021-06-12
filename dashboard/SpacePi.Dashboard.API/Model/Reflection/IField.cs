using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Reflection {
    public interface IField : INotifyPropertyChanged, INotifyCollectionChanged {
        string Name { get; }

        bool IsList { get; }

        public static readonly PropertyChangedEventArgs CountChanged = new(nameof(Count));
        int Count { get; }

        void Append();

        void Remove(int idx);
    }
}
