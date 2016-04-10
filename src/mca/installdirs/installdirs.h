/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2006-2015 Los Alamos National Security, LLC.  All rights
 *                         reserved.
 * Copyright (c) 2016      Intel, Inc. All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#ifndef PMIX_MCA_INSTALLDIRS_INSTALLDIRS_H
#define PMIX_MCA_INSTALLDIRS_INSTALLDIRS_H

#include <src/include/pmix_config.h>

#include "src/mca/mca.h"
#include "src/mca/base/base.h"

BEGIN_C_DECLS

/*
 * Most of this file is just for ompi_info.  The only public interface
 * once pmix_init has been called is the pmix_install_dirs structure
 * and the pmix_install_dirs_expand() call */
struct pmix_install_dirs_t {
    char* prefix;
    char* exec_prefix;
    char* bindir;
    char* sbindir;
    char* libexecdir;
    char* datarootdir;
    char* datadir;
    char* sysconfdir;
    char* sharedstatedir;
    char* localstatedir;
    char* libdir;
    char* includedir;
    char* infodir;
    char* mandir;

    /* Note that the following fields intentionally have an "ompi"
       prefix, even though they're down in the PMIX layer.  This is
       not abstraction break because the "ompi" they're referring to
       is for the build system of the overall software tree -- not an
       individual project within that overall tree.

       Rather than using pkg{data,lib,includedir}, use our own
       ompi{data,lib,includedir}, which is always set to
       {datadir,libdir,includedir}/openmpi. This will keep us from
       having help files in prefix/share/open-rte when building
       without Open MPI, but in prefix/share/openmpi when building
       with Open MPI.

       Note that these field names match macros set by configure that
       are used in Makefile.am files.  E.g., project help files are
       installed into $(pmixdatadir). */
    char* pmixdatadir;
    char* pmixlibdir;
    char* pmixincludedir;
};
typedef struct pmix_install_dirs_t pmix_install_dirs_t;

/* Install directories.  Only available after pmix_init() */
PMIX_DECLSPEC extern pmix_install_dirs_t pmix_install_dirs;

/**
 * Expand out path variables (such as ${prefix}) in the input string
 * using the current pmix_install_dirs structure */
PMIX_DECLSPEC char * pmix_install_dirs_expand(const char* input);


/**
 * Structure for installdirs components.
 */
struct pmix_installdirs_base_component_2_0_0_t {
    /** MCA base component */
    mca_base_component_t component;
    /** MCA base data */
    mca_base_component_data_t component_data;
    /** install directories provided by the given component */
    pmix_install_dirs_t install_dirs_data;
};
/**
 * Convenience typedef
 */
typedef struct pmix_installdirs_base_component_2_0_0_t pmix_installdirs_base_component_t;

/*
 * Macro for use in components that are of type installdirs
 */
#define PMIX_INSTALLDIRS_BASE_VERSION_2_0_0 \
    PMIX_MCA_BASE_VERSION_2_1_0("installdirs", 2, 0, 0)

END_C_DECLS

#endif /* PMIX_MCA_INSTALLDIRS_INSTALLDIRS_H */
