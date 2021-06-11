using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    public interface IGroupNode : INode {
        IEnumerable<IActionNode> Actions { get; }
        IEnumerable<IGroupNode> SubGroups { get; }
        IEnumerable<IValueNode> Values { get; }
    }
}
