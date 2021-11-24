using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model {
    [Plugin("SpacePi.Dashboard.API.Model", "3.0.0", -9_000_000)]
    public class ModelPlugin : Plugin {
        public DashboardModel Model { get; } = new();
    }
}
