#
# Copyright 2015-2016 Intel, Inc. All rights reserved
#
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#

# Only install the headers if we're in standalone mode

EXTRA_DIST = private/autogen/README.txt

if ! PMIX_EMBEDDED_MODE
include_HEADERS = \
        pmix.h \
        pmix_server.h \
        pmi.h \
        pmi2.h

headers = \
        private/align.h \
        private/hash_string.h \
        private/pmix_socket_errno.h \
        private/pmix_stdint.h \
        private/prefetch.h \
        private/types.h
include_pmixdir = $(includedir)/pmix
include_pmix_HEADERS = \
    pmix/rename.h \
    pmix/pmix_common.h

include_pmix_autogendir = $(includedir)/pmix/autogen
include_pmix_autogen_HEADERS = \
        pmix/autogen/pmix_config_top.h \
        pmix/autogen/pmix_config_bottom.h
nodist_include_pmix_autogen_HEADERS = \
        pmix/autogen/config.h
include_private_autogendir = $(includedir)/private/autogen
nodist_include_private_autogen_HEADERS = \
        private/autogen/config.h

endif ! PMIX_EMBEDDED_MODE
