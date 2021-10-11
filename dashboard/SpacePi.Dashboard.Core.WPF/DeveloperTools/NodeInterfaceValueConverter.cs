using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace SpacePi.Dashboard.Core.WPF.DeveloperTools {
    class NodeInterfaceValueConverter : IMultiValueConverter {
        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture) =>
            values.Cast<IEnumerable>()
            .Where(e => e != null)
            .SelectMany(e => e.Cast<object>())
            .ToList();

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture) => throw new NotSupportedException();
    }
}
