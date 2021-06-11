﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    abstract class ScalarNode<TField> : ScalarNodeBase<TField>, INode where TField : IField {
        private string _Name;

        public static readonly PropertyChangedEventArgs NameChanged = new(nameof(Name));
        public string Name {
            get => _Name;
            private set {
                if (_Name != value) {
                    _Name = value;
                    PropertyChanged?.Invoke(this, NameChanged);
                }
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        protected void FirePropertyChanged(PropertyChangedEventArgs e) => PropertyChanged?.Invoke(this, e);

        public override void Reload() => Name = Field.IsList ? $"{Field.Name}[{Index}]" : Field.Name;

        public ScalarNode(TField field, int index) : base(field, index) {
        }
    }
}
