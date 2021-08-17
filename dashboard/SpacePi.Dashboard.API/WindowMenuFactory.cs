using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.API {
    [BindingFactory(typeof(WindowMenuItemAttribute), typeof(BindWindowMenuAttribute), typeof(WindowMenuItem))]
    public class WindowMenuFactory {
        [Subfactory]
        public IBoundFactory<PluginFactory> NextFactory { get; set; }

        [BindingFactoryArray(typeof(WindowMenuItem))]
        public WindowMenuItem[] MenuItems { get; set; }

        [BindingFactoryBoxer]
        public WindowMenuItem Box(
            [BindingParameter("id")] string id,
            [BindingParameter("name")] string name) => new(id, name);

        [BindingFactoryUnboxer]
        public WindowMenuItem Unbox(WindowMenuItem item) => item;

        [BindingFactoryLoader(1_000_000_000)]
        public void LoadNext() => NextFactory.Create();
    }
}
