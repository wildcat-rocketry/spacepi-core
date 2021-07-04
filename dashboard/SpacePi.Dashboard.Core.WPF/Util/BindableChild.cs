using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace SpacePi.Dashboard.Core.WPF.Util {
    public class BindableChild : ContentControl {
        public static readonly DependencyProperty ChildProperty = DependencyProperty.Register(nameof(Child), typeof(Visual), typeof(BindableChild), new FrameworkPropertyMetadata(ChildChanged));

        public Visual Child {
            get => (Visual) GetValue(ChildProperty);
            set => SetValue(ChildProperty, value);
        }

        private static void ChildChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e) {
            if (sender is BindableChild c && e.OldValue != e.NewValue) {
                c.Content = e.NewValue;
            }
        }

        public BindableChild() {
            Content = Child;
        }
    }
}
