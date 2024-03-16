Vendor Integration Guide
========================

The purpose of this guide is to provide vendors with a roadmap
on how to integrate PMIx support into their system software stack.
While there are quite a few PMIx APIs, almost all of those target
the application and tool use-cases. Only a small number actually
involve the host environment.

The information is broken down into functional blocks so that
a vendor can target their support. Each section describes
the responsibilities of the host environment when supporting
that functional block, plus the APIs and attributes involved.
Examples are provided where applicable.

.. toctree::
   :maxdepth: 2

   definitions.rst
   overview.rst
   core.rst
   wireup.rst
   events.rst
   tools.rst
   groups.rst
