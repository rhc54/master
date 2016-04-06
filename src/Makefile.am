#
# Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
#                         University Research and Technology
#                         Corporation.  All rights reserved.
# Copyright (c) 2004-2005 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
# Copyright (c) 2004-2009 High Performance Computing Center Stuttgart,
#                         University of Stuttgart.  All rights reserved.
# Copyright (c) 2004-2005 The Regents of the University of California.
#                         All rights reserved.
# Copyright (c) 2006-2015 Cisco Systems, Inc.  All rights reserved.
# Copyright (c) 2012-2013 Los Alamos National Security, Inc.  All rights reserved.
# Copyright (c) 2013-2016 Intel, Inc. All rights reserved
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#

# Note that the -I directory must *exactly* match what was specified
# via AC_CONFIG_MACRO_DIR in configure.ac.
ACLOCAL_AMFLAGS = -I ./config

SUBDIRS = \
    mca/base \
    $(MCA_src_FRAMEWORKS_SUBDIRS) \
    $(MCA_src_FRAMEWORK_COMPONENT_STATIC_SUBDIRS) \
    . \
    $(MCA_src_FRAMEWORK_COMPONENT_DSO_SUBDIRS)

DIST_SUBDIRS = \
    mca/base \
    $(MCA_src_FRAMEWORKS_SUBDIRS) \
    $(MCA_src_FRAMEWORK_COMPONENT_ALL_SUBDIRS)

headers =
sources =
nodist_headers =
EXTRA_DIST =

include class/Makefile.am
include include/Makefile.am
include mca/Makefile.am
include util/Makefile.am
include usock/Makefile.am
include client/Makefile.am
include server/Makefile.am
include sec/Makefile.am
include common/Makefile.am

if WANT_DSTORE
include sm/Makefile.am
include dstore/Makefile.am
endif

if PMIX_EMBEDDED_MODE
noinst_LTLIBRARIES = libpmix.la
libpmix_la_SOURCES = $(headers) $(sources)
libpmix_la_LDFLAGS =

else

lib_LTLIBRARIES = libpmix.la
libpmix_la_SOURCES = $(headers) $(sources)
libpmix_la_LDFLAGS = -version-info $(libpmix_so_version)

endif !PMIX_EMBEDDED_MODE
