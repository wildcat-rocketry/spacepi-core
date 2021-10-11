using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using SpacePi.Dashboard.Core.DeveloperTools;

namespace SpacePi.Dashboard.Core.WPF.DeveloperTools {
    /// <summary>
    /// Interaction logic for DeveloperTools.xaml
    /// </summary>
    public partial class DeveloperTools : UserControl {
        public DeveloperTools() => InitializeComponent();

        private void PerformAction(object sender, RoutedEventArgs e) => ((IActionNode) ((FrameworkElement) sender).DataContext).PerformAction();
    }
}
