Working with Submodules
=======================

The SpacePi code is split into multiple repositories using the concept of a Git submodule.
This page is intended to give a basic overview of how to work with submodules well.

Committing Repositories
-----------------------

When getting ready to commit or running :code:`git status`, the submodule repositories may show up as files.
These "files" that are committed contain the commit ID of the submodule repository.
This way, there is history of which version of dependent repositories this version of the code was using.

In general, each commit should also commit any (if any) changes to the submodule repositories.
Then, when the repository is pulled on another device, that other device will update to the submodule commit ID referenced in the parent commit ID.

If the status says a submodule has changes inside it, that means files inside the submodule changed, so they would have to be committed first.
One commit cannot span across submodules since they are fully separate repositories, however a single commit should generally not need to span across submodules.
Therefore, if there are changes in multiple submodules, it would be advisable to take another look at all of the changes to ensure the desired files, and only the desired files, were changed.

Pulling New Changes
-------------------

Without submodules, to pull new changes, it was simple:

.. code-block:: text

    $ git pull

However, this will not entirely work with submodules.
Instead, the following can be used:

.. code-block:: text

    $ git pull
    $ git submoudle update --init --recursive --remote

The options to :code:`git submodule update` are explained below:

* :code:`--init`: Clone any submodules which have not been cloned
* :code:`--recursive`: Update all submodules in tree, not just directly at this level
* :code:`--remote`: Update each submodule to the :code:`master` branch rather than the commit ID stored in the pulled commit

This can be short-cutted using the following command:

.. code-block:: text

    $ git config --global alias.update '!f(){ git pull "$@" && git submoudle update --init --recursive --remote; };f'

Then, to pull and update all submodules, simply run this:

.. code-block:: text

    $ git update

Working on Submodules
---------------------

In order to reduce the number of cloned repositories, it is possible to clone only the packages repository and do all work in the other submodules.
For example, one could clone the packages repo and work on the target repo since the target repo is a submodule of the packages repo.
This is also beneficial because then the new target changes could be tested in the packages repo before being committed.

If working on a submodule, there are a few things to remember, however:

1. Before starting a change, run :code:`git checkout master` to make sure the :code:`HEAD` is attached, as :code:`git submodule update --init --recursive --remote` will detach the :code:`HEAD`
2. Make sure to be in the correct repository/directory when committing changes.
   If testing in one repository and changing code in another like the above example, maybe open two terminals: one for testing and one for Git.
3. One build folder in the root of the packages repository can function as build folders for each individual submodule also.
   The directory structure in the repository is mirrored in the build folder, and one can run :code:`make` in a subdirectory of the build folder to build a smaller amount of code rather than the whole repo tree.
