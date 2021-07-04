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
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.MainWindow {
    public partial class MainWindow : Window {
        public MainWindow(IEnumerable<IContext> contexts) {
            InitializeComponent();
            Closed += (_, _) => {
                foreach (IContext context in contexts) {
                    context.Dispose();
                }
            };
        }
    }
}
