using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API {
    public interface IPluginFactory {
        IPlugin[] Plugins { get; }

        IContext CreateContext();

        public static IPluginFactory Instance;
    }
}
