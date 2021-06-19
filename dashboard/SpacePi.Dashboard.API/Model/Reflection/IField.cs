using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Reflection {
    public interface IField : INotifyCollectionChanged {
        string Name { get; }

        bool IsList { get; }

        int Count { get; }

        int Number { get; }

        bool Transient { get; }

        void Append();

        void Remove(int idx);
    }
}
