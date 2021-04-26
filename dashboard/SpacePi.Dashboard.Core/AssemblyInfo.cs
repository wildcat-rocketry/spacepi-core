using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Core {
    static class AssemblyInfo {
        public static Version Version = Assembly.GetExecutingAssembly().GetName().Version;
    }
}
