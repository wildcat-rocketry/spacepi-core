﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class ReloadNode : IActionNode {
        private readonly IReloadable Obj;

        public string Name { get; }

        public event PropertyChangedEventHandler PropertyChanged { add { } remove { } }

        public void PerformAction() => Obj.Reload();

        public void Dispose() {
        }

        public ReloadNode(string name, IReloadable obj) {
            Obj = obj;
            Name = name;
        }
    }
}
