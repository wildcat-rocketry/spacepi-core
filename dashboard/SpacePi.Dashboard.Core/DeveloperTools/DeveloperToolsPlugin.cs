using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.API.Model;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class TestClass : ModelClass, IObject {
        public IClass Reflection => this;

        public TestClass(string name, params IField[] fields) : base(name, fields) {
        }
    }

    [Plugin("SpacePi.Dashboard.Core.DeveloperTools", "3.0.0", 11_002_000)]
    public class DeveloperToolsPlugin : Plugin {
        public IGroupNode Root { get; private set; }

        [BindPlugin]
        public ModelPlugin Model {
            set {
                ObservableCollection<int> myIntList = new();
                myIntList.Add(1);
                myIntList.Add(2);
                myIntList.Add(4);
                int enumField = 1;
                double scalarDouble = 1.23;
                TestClass subClassField = new("SubClassField",
                    new ScalarEnumField("EnumField", 0, false, new ModelEnum("Fruit", (0, "Apple"), (1, "Banana"), (2, "Cherry")), () => enumField, v => enumField = v),
                    new ScalarPrimitiveField("ScalarDouble", 0, false, IPrimitiveField.Types.Double, () => scalarDouble, v => scalarDouble = (double) v)
                );
                TestClass model = new("TestingModelClass",
                    new VectorPrimitiveField<int>("MyIntList", 0, false, IPrimitiveField.Types.Int32, myIntList),
                    new ScalarClassField<TestClass>("SubClassField", 0, false, () => subClassField, v => subClassField = v)
                );
                Root = new ScalarClassNode(new ScalarClassField<TestClass>("Dashboard Model", 0, false, () => model, _ => throw new NotSupportedException()));
            }
        }
    }
}
