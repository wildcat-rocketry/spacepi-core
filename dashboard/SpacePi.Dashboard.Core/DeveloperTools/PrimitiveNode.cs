using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class PrimitiveNode : ValueNode<IPrimitiveField> {
        public override string Name => "Value";

        protected override bool TryParse(string str) {
            if (IPrimitiveField.TryParsers[(int)Field.Type](str, out object val)) {
                Field[Index] = val;
                return true;
            }
            return false;
        }

        protected override string ValueToString() => IPrimitiveField.ToStringers[(int) Field.Type](Field[Index]);

        public PrimitiveNode(IPrimitiveField field, int index = 0) : base(field, index) {
        }
    }
}
