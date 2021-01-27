import subprocess
import os

author                  = "Zach Deibert"
breathe_default_project = "spacepi"
breathe_projects        = { "spacepi": os.environ.get("BUILD_DIR", ".") + "/xml" }
extensions              = [ "breathe" ]
master_doc              = "index"
project                 = "SpacePi Core"
release                 = "2.0"
source_suffix           = ".rst"
version                 = "2.0"

if os.environ.get("READTHEDOCS") == "True":
    with open("Doxyfile.in", "rt") as fin:
        with open("Doxyfile", "wt") as fout:
            for line in fin:
                fout.write(line.replace("${CMAKE_CURRENT_SOURCE_DIR}", "..").replace("${CMAKE_CURRENT_BINARY_DIR}", "../docs"))
    subprocess.call("cd ../include; doxygen ../docs/Doxyfile", shell=True)
else:
    import sphinx_rtd_theme
    html_theme      = "sphinx_rtd_theme"
    html_theme_path = [ sphinx_rtd_theme.get_html_theme_path() ]
