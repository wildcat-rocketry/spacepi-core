using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.API {
    [BindingFactory(typeof(SuperPluginAttribute), typeof(BindSuperPluginAttribute)), EntryPoint]
    public class SuperPluginFactory : PluginFactory {
        [Subfactory]
        public IBoundFactory<WindowMenuFactory> NextFactory { get; set; }

        [BindingFactoryLoader(1_000_000_000)]
        public void LoadNext() => NextFactory.Create();
    }
}
