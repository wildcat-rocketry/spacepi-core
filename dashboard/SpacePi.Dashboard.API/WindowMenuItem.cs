using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API {
    public class WindowMenuItem {
        public string Id { get; }
        public string Name { get; }

        [BindWindowMenu(BindWindowMenu.MatchID, "parent")]
        public IEnumerable<WindowMenuItem> Children { get; set; }

        public WindowMenuItem(string id, string name) {
            Id = id;
            Name = name;
        }
    }
}
