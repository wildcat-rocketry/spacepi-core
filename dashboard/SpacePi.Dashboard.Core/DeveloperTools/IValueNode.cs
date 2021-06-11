using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    public interface IValueNode : INode {
        string Value { get; set; }

        bool Valid { get; }
    }
}
