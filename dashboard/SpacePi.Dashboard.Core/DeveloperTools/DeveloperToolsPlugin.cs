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
    [Plugin("SpacePi.Dashboard.Core.DeveloperTools", "3.0.0", 11_002_000)]
    public class DeveloperToolsPlugin : Plugin {
        public IGroupNode Root { get; private set; }

        [BindPlugin]
        public ModelPlugin Model {
            set => Root = new ScalarClassNode(new ScalarClassField<DashboardModel>("Dashboard Model", 0, false, () => value.Model, _ => throw new NotSupportedException()));
        }
    }
}
