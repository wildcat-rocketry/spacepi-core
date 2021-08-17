using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    public interface IValueNode : INode {
        public static readonly PropertyChangedEventArgs ValueChanged = new(nameof(Value));
        string Value { get; set; }

        public static readonly PropertyChangedEventArgs ValidChanged = new(nameof(Valid));
        bool Valid { get; }
    }
}
