Contributing Core Code
======================

Adding New Classes
------------------

1. Create a header file under :code:`/include/spacepi/{module}/{ClassName}.hpp`:

   .. code-block:: cpp

       #ifndef SPACEPI_CORE_{MODULE}_{CLASSNAME}_HPP
       #define SPACEPI_CORE_{MODULE}_{CLASSNAME}_HPP

       namespace spacepi {
           namespace {module} {
               class ClassName {
               };
           }
       }

       #endif

2. Create implementation file under :code:`/src/{module}/{ClassName}.cpp`
3. Edit :code:`/include/spacepi/{Module}.hpp` to include new class header

   .. code-block:: cpp

       #include <spacepi/{module}/{FirstAlphabeticFile}.hpp>
       #include <spacepi/{module}/{NextAlphabeticFile}.hpp>
       // ...
       #include <spacepi/{module}/{ClassName}.hpp>
       // ...
       #include <spacepi/{module}/{LastAlphabeticFile}.hpp>

       namespace spacepi {
           /**
            * \brief {Module description}
            */
           namespace {module} {}
       }

4. Create new documentation wrapper for the class under :code:`/docs/spacepi/{module}/{ClassName}.rst`:

   .. code-block:: rst

       spacepi::{module}::{ClassName}
       ==============================

       .. doxygenclass:: spacepi::{module}::{ClassName}
           :members:
           :protected-members:

5. Register class documentation with namespace in :code:`/docs/spacepi/{Module}.rst`:

   .. code-block:: rst

       spacepi::{module} namespace
       ===========================

       .. toctree::
           {module}/{FirstAlphabeticFile}
           {module}/{NextAlphabeticFile}
           {...}
           {module}/{ClassName}
           {...}
           {module}/{LastAlphabeticFile}

5. Edit :code:`/CMakeLists.txt` to register the new class:

   .. code-block:: cmake

       # ...

       add_library(spacepi SHARED
           src/{firstAlphabeticModule}/{FirstAlphabeticFile}.cpp
           # ...
           src/{module}/{ClassName}.cpp
           # ...
           src/{lastAlphabeticModule}/{LastAlphabeticFile}.cpp
       )

       # ...

           add_custom_command(
               OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/doxygen.stamp"
               # ...
               DEPENDS
                   "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
                   include/spacepi/{firstAlphabeticModule}/{FirstAlphabeticFile}.hpp
                   # ...
                   include/spacepi/{module}/{ClassName}.hpp
                   # ...
                   include/spacepi/{lastAlphabeticModule}/{LastAlphabeticFile}.hpp
                # ...
           )

           # ...

               add_custom_command(
                   OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp"
                   # ...
                   DEPENDS
                       # ...
                       docs/spacepi/{firstAlphabeticModule}/{FirstAlphabeticFile}.rst
                       # ...
                       docs/spacepi/{module}/ClassName}.rst
                       # ...
                       docs/spacepi/{lastAlphabeticModule}/{LastAlphabeticFile}.rst
                    # ...
               )

       # ...

Adding New Documentation Pages
------------------------------

1. Create documentation file :code:`/docs/detailed/{short-name}.rst`

   .. code-block:: rst

       {Full Page Name}
       ================

       Page content...

2. Edit :code:`/docs/index.rst` to register page:

   .. code-block:: rst

       SpacePi Core documentation
       ==========================

       .. toctree::
           detailed/{first-logical-page}
           detailed/{next-logical-page}
           {...}
           detailed/{short-name}
           {...}
           detailed/{last-logical-page}
           SpacePi

3. Edit :code:`/CMakeLists.txt` to register the new class:

   .. code-block:: cmake


       # ...

               add_custom_command(
                   OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp"
                   # ...
                   DEPENDS
                       # ...
                       docs/detailed/{first-alphabetic-page}.rst
                       docs/detailed/{next-alphabetic-page}.rst
                       # ...
                       docs/detailed/{short-name}.rst
                       # ...
                       docs/detailed/{last-alphabetic-page}.rst
                    # ...
               )

       # ...

Versioning
----------

The core library needs to keep track of its version number to make sure each submodule contains compatible versions while building.
To do this, the following versioning scheme has been adopted:

1. The version number is three numbers separated by :code:`.` s
2. The first number is incremented every time the framework is rewritten mostly ground-up
3. The second number is incremented when new features are added
4. The third number is incremented when a backwards-compatible but not forwards-compatible change is made
5. When one number is incremented, all further numbers reset to 0

The full current version should be stored in :code:`/cmake/Version.cmake`.
The first two digits should be stored in :code:`/docs/conf.py`.

The latest commit for each version should be tagged in Git with the version ID.
Tags should be of the format :code:`vXX.XX.XX`.

Before Committing
-----------------

Before committing to core, the project should be built with :code:`make` to ensure nothing is broken.
In order to ensure full compilation, all optional packages should be installed, and example code should also be built.
