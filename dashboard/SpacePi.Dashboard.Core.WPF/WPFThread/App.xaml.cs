﻿using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace SpacePi.Dashboard.Core.WPF.WPFThread {
    public partial class App : Application {
        internal bool[] IsThreadStarted { private get; init; }

        private void AppStartup(object sender, StartupEventArgs e) {
            lock (IsThreadStarted) {
                IsThreadStarted[0] = true;
                Monitor.PulseAll(IsThreadStarted);
            }
        }
    }
}
