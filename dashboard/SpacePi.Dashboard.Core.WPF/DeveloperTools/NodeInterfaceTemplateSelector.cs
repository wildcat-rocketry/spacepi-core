using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using SpacePi.Dashboard.Core.DeveloperTools;

namespace SpacePi.Dashboard.Core.WPF.DeveloperTools {
    class NodeInterfaceTemplateSelector : DataTemplateSelector {
        public DataTemplate ActionNodeTemplate { get; set; }
        public DataTemplate GroupNodeTemplate { get; set; }
        public DataTemplate ValueNodeTemplate { get; set; }

        public override DataTemplate SelectTemplate(object item, DependencyObject container) {
            if (item is IActionNode) {
                return ActionNodeTemplate;
            }
            if (item is IGroupNode) {
                return GroupNodeTemplate;
            }
            if (item is IValueNode) {
                return ValueNodeTemplate;
            }
            return base.SelectTemplate(item, container);
        }
    }
}
