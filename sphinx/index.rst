.. Odil documentation master file, created by
   sphinx-quickstart on Wed Jan 20 20:38:35 2016.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to Odil's documentation!
================================

Odil is a C++11 library for the `DICOM <http://dicom.nema.org/>`_ standard with
Python wrappers.

Odil leverages C++ constructs to provide a user-friendly API of the different
parts of the `DICOM <http://dicom.nema.org/>`_ standard. Included in Odil are
exception-based error handling, generic access to datasets elements, standard 
JSON and XML representation of datasets, and generic implementation of 
messages, clients and servers for the various DICOM protocols.

Odil also provides conversion to and from 
`DCMTK <http://dicom.offis.de/dcmtk.php.en>`_ data structures.

Odil builds and run on:

* Linux (Debian 7 and 8, Ubuntu 12.04, 14.04, and 16.04, all 32 and 64 bits). 
  Official packages are available (`Debian`_, `Ubuntu`_), as well as 
  `unofficial backports`_.
* OS X

Presentation given at `Hacking Health 2016`_: `PDF`_

Contents:

.. toctree::
   :maxdepth: 2

.. _Debian: https://packages.debian.org/search?keywords=odil&searchon=sourcenames&suite=all&section=all
.. _Hacking Health 2016: http://hackinghealth.ca/fr/event/hacking-health-camp-formation/
.. _Ubuntu: http://packages.ubuntu.com/search?keywords=odil&searchon=sourcenames&suite=all&section=all
.. _unofficial backports: https://github.com/lamyj/packages
.. _PDF: odil_hacking_health_2016.pdf
