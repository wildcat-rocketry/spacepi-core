using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    public interface INode : INotifyPropertyChanged, IDisposable {
        public static readonly PropertyChangedEventArgs NameChanged = new(nameof(Name));
        string Name { get; }
    }
}
