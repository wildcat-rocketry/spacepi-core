using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class ListRemoveNode : IActionNode {
        private readonly IField Field;
        private readonly int Index;

        public string Name => "Remove Item";

        public event PropertyChangedEventHandler PropertyChanged;

        public void PerformAction() => Field.Remove(Index);

        public ListRemoveNode(IField field, int index) {
            Field = field;
            Index = index;
        }
    }
}
