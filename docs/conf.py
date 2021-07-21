#!/usr/bin/env python3
import os
import pathlib
import shutil
import subprocess
import xml.etree.ElementTree

spacepi_core_docs_builddir = pathlib.Path(os.environ.get("BUILD_DIR", "."))
author                     = "Wildcat Rocketry Club"
breathe_default_project    = "spacepi"
breathe_projects           = { "spacepi": spacepi_core_docs_builddir / "xml" }
extensions                 = [ "breathe" ]
master_doc                 = "rst/index"
project                    = "SpacePi Core"
release                    = "3.0"
source_suffix              = ".rst"
version                    = "3.0"

def spacepi_core_docs_setup():
    builddir = spacepi_core_docs_builddir
    doxygen = os.environ.get("DOXYGEN_EXE", "doxygen")
    srcroot = pathlib.Path("..").resolve()

    def generateClass(refid: str, name: str) -> str:
        rst = open(builddir / "rst" / f"{refid}.rst", "w")
        rst.write(f"""
{name}
{'=' * len(name)}

.. doxygenclass:: {name}
    :members:
    :protected-members:
"""[1:])
        rst.close()
        return refid

    def generateNamespace(refid: str) -> str:
        tree = xml.etree.ElementTree.parse(builddir / "xml" / f"{refid}.xml")
        rst = open(builddir / "rst" / f"{refid}.rst", "w")
        name = f"""{tree.find("./compounddef/compoundname").text} Namespace"""
        rst.write(f"""
{name}
{'=' * len(name)}

.. toctree::
"""[1:])
        for innernamespace in tree.findall("./compounddef/innernamespace"):
            rst.write(f"""
    {generateNamespace(innernamespace.attrib["refid"])}
"""[1:])
        for innerclass in tree.findall("./compounddef/innerclass"):
            rst.write(f"""
    {generateClass(innerclass.attrib["refid"], innerclass.text)}
"""[1:])
        rst.close()
        return refid

    (builddir / "rst").mkdir(parents=True, exist_ok=True)
    doxyfile = open(builddir / "Doxyfile", "w")
    doxyfile.write(f"""
@INCLUDE         = "{srcroot / "docs" / "Doxyfile.in"}"
INCLUDE_PATH     = "{srcroot / "include"}"
OUTPUT_DIRECTORY = "{builddir}"
INPUT            = "{srcroot / "include"}"
"""[1:])
    doxyfile.close()
    subprocess.run([doxygen, builddir / "Doxyfile"])
    for old in (builddir / "rst").glob("*.rst"):
        os.remove(old)
    generateNamespace("namespacespacepi")
    index = open(builddir / "rst" / "index.rst", "w")
    index.write("""
SpacePi Core Documentation
==========================

.. toctree::
"""[1:])
    for staticdoc in (srcroot / "docs").glob("*.rst"):
        shutil.copy(staticdoc, builddir / "rst" / staticdoc.name)
        index.write(f"""
    {os.path.splitext(staticdoc.name)[0]}
"""[1:])
    index.write("""
    namespacespacepi
"""[1:])
    index.close()

spacepi_core_docs_setup()
if os.environ.get("READTHEDOCS") != "True":
    import sphinx_rtd_theme
    html_theme      = "sphinx_rtd_theme"
    html_theme_path = [ sphinx_rtd_theme.get_html_theme_path() ]
