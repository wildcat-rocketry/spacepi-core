using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    abstract class AutoScalarGroupNodeHelper<TField> : ScalarGroupNode<TField> where TField : IField {
        protected abstract IEnumerable<IGroupNode> _SubGroups { get; }

        protected abstract IEnumerable<IValueNode> _Values { get; }

        public override IEnumerable<IGroupNode> SubGroups => _SubGroups;

        public override IEnumerable<IValueNode> Values => _Values;

        public AutoScalarGroupNodeHelper(TField field, int index, string reloadName) : base(field, index, reloadName) {
        }
    }

    abstract class AutoScalarGroupNode<TField> : AutoScalarGroupNodeHelper<TField> where TField : IField {
        public new IEnumerable<IGroupNode> SubGroups { get; protected set; }

        public new IEnumerable<IValueNode> Values { get; protected set; }

        protected override IEnumerable<IGroupNode> _SubGroups => SubGroups;

        protected override IEnumerable<IValueNode> _Values => Values;

        public AutoScalarGroupNode(TField field, int index, string reloadName) : base(field, index, reloadName) {
        }
    }
}
