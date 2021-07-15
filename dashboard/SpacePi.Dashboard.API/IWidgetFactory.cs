﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API {
    public interface IWidgetFactory {
    }

    public interface IWidgetFactory<TGfxWidget> : IWidgetFactory {
        IWidgetViewModelFactory ViewModelFactory { get; }

        TGfxWidget CreateWidget(object viewModel);
    }
}
