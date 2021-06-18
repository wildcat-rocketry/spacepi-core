using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.DeveloperTools {
    public class DeveloperToolsContext : CoreContext {
        [BindContext]
        public Core.DeveloperTools.DeveloperToolsContext Core;

        public UserControl ViewComponent => new DeveloperTools() {
            DataContext = new[] { Core.Root }
        };
    }
}
