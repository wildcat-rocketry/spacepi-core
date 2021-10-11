using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class EnumNamedNode : ValueNode<IEnumField> {
        public override string Name => "Named Value";

        protected override bool TryParse(string str) {
            if (Field.Type.NamedToOrdinal(str, out int ordinal)) {
                Field[Index] = ordinal;
                return true;
            }
            return false;
        }

        protected override string ValueToString() {
            Field.Type.OrdinalToNamed(Field[Index], out string named);
            return named;
        }

        public EnumNamedNode(IEnumField field, int index) : base(field, index) {
        }
    }
}
