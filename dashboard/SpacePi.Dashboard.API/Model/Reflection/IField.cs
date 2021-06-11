using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Reflection {
    public interface IField {
        string Name { get; }

        bool IsList { get; }

        int Count { get; }

        void Append();

        void Remove(int idx);
    }
}
