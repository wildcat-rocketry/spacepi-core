using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Reflection {
    public interface IEnum {
        string Name { get; }

        IEnumerable<string> ValueNames { get; }

        bool OrdinalToNamed(int ordinal, out string named);

        bool NamedToOrdinal(string named, out int ordinal);
    }
}
