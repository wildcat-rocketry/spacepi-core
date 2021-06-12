using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class EnumOrdinalNode : ValueNode<IEnumField> {
        public override string Name => "Ordinal Value";

        protected override bool TryParse(string str) {
            if (int.TryParse(Value, out int v)) {
                Field[Index] = v;
                return true;
            }
            return false;
        }

        protected override string ValueToString() => Field[Index].ToString();

        public EnumOrdinalNode(IEnumField field, int index) : base(field, index) {
        }
    }
}
