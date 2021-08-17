﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Reflection {
    public interface IEnumField : IField {
        IEnum Type { get; }

        int this[int idx] { get; set; }
    }
}
