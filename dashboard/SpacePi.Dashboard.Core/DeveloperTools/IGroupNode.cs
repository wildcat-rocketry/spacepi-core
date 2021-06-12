using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    public interface IGroupNode : INode {
        public static readonly PropertyChangedEventArgs ActionsChanged = new(nameof(Actions));
        IEnumerable<IActionNode> Actions { get; }

        public static readonly PropertyChangedEventArgs SubGroupsChanged = new(nameof(SubGroups));
        IEnumerable<IGroupNode> SubGroups { get; }

        public static readonly PropertyChangedEventArgs ValuesChanged = new(nameof(Values));
        IEnumerable<IValueNode> Values { get; }
    }
}
