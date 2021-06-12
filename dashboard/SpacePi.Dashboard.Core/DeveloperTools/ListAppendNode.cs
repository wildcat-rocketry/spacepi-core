using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class ListAppendNode : IActionNode {
        private readonly IField Field;

        public string Name => "Append Item";

        public event PropertyChangedEventHandler PropertyChanged { add { } remove { } }

        public void PerformAction() => Field.Append();

        public void Dispose() {
        }

        public ListAppendNode(IField field) => Field = field;
    }
}
