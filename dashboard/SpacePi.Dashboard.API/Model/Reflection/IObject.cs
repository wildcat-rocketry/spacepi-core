using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Reflection {
    public interface IObject {
        IClass Reflection { get; }

        void Serialize(Stream s);

        void Parse(Stream s);
    }
}
