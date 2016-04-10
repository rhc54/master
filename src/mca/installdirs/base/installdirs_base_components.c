/*
 * Copyright (c) 2006-2012 Los Alamos National Security, LLC.  All rights
 *                         reserved.
 * Copyright (c) 2007      Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2010      Sandia National Laboratories. All rights reserved.
 * Copyright (c) 2015      Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
 * Copyright (c) 2016      Intel, Inc. All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 */

#include <src/include/pmix_config.h>

#include "pmix/pmix_common.h"
#include "src/mca/mca.h"
#include "src/mca/installdirs/installdirs.h"
#include "src/mca/installdirs/base/base.h"
#include "src/mca/installdirs/base/static-components.h"

pmix_install_dirs_t pmix_install_dirs = {0};

#define CONDITIONAL_COPY(target, origin, field)                 \
    do {                                                        \
        if (origin.field != NULL && target.field == NULL) {     \
            target.field = origin.field;                        \
        }                                                       \
    } while (0)

static int
pmix_installdirs_base_open(mca_base_open_flag_t flags)
{
    mca_base_component_list_item_t *component_item;
    int ret;

    ret = mca_base_framework_components_open (&pmix_installdirs_base_framework,
                                              flags);
    if (PMIX_SUCCESS != ret) {
        return ret;
    }

    PMIX_LIST_FOREACH(component_item, &pmix_installdirs_base_framework.framework_components, mca_base_component_list_item_t) {
        const pmix_installdirs_base_component_t *component =
            (const pmix_installdirs_base_component_t *) component_item->cli_component;

        /* copy over the data, if something isn't already there */
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         prefix);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         exec_prefix);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         bindir);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         sbindir);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         libexecdir);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         datarootdir);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         datadir);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         sysconfdir);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         sharedstatedir);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         localstatedir);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         libdir);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         includedir);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         infodir);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         mandir);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         pmixdatadir);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         pmixlibdir);
        CONDITIONAL_COPY(pmix_install_dirs, component->install_dirs_data,
                         pmixincludedir);
    }

    /* expand out all the fields */
    pmix_install_dirs.prefix =
        pmix_install_dirs_expand_setup(pmix_install_dirs.prefix);
    pmix_install_dirs.exec_prefix =
        pmix_install_dirs_expand_setup(pmix_install_dirs.exec_prefix);
    pmix_install_dirs.bindir =
        pmix_install_dirs_expand_setup(pmix_install_dirs.bindir);
    pmix_install_dirs.sbindir =
        pmix_install_dirs_expand_setup(pmix_install_dirs.sbindir);
    pmix_install_dirs.libexecdir =
        pmix_install_dirs_expand_setup(pmix_install_dirs.libexecdir);
    pmix_install_dirs.datarootdir =
        pmix_install_dirs_expand_setup(pmix_install_dirs.datarootdir);
    pmix_install_dirs.datadir =
        pmix_install_dirs_expand_setup(pmix_install_dirs.datadir);
    pmix_install_dirs.sysconfdir =
        pmix_install_dirs_expand_setup(pmix_install_dirs.sysconfdir);
    pmix_install_dirs.sharedstatedir =
        pmix_install_dirs_expand_setup(pmix_install_dirs.sharedstatedir);
    pmix_install_dirs.localstatedir =
        pmix_install_dirs_expand_setup(pmix_install_dirs.localstatedir);
    pmix_install_dirs.libdir =
        pmix_install_dirs_expand_setup(pmix_install_dirs.libdir);
    pmix_install_dirs.includedir =
        pmix_install_dirs_expand_setup(pmix_install_dirs.includedir);
    pmix_install_dirs.infodir =
        pmix_install_dirs_expand_setup(pmix_install_dirs.infodir);
    pmix_install_dirs.mandir =
        pmix_install_dirs_expand_setup(pmix_install_dirs.mandir);
    pmix_install_dirs.pmixdatadir =
        pmix_install_dirs_expand_setup(pmix_install_dirs.pmixdatadir);
    pmix_install_dirs.pmixlibdir =
        pmix_install_dirs_expand_setup(pmix_install_dirs.pmixlibdir);
    pmix_install_dirs.pmixincludedir =
        pmix_install_dirs_expand_setup(pmix_install_dirs.pmixincludedir);

#if 0
    fprintf(stderr, "prefix:         %s\n", pmix_install_dirs.prefix);
    fprintf(stderr, "exec_prefix:    %s\n", pmix_install_dirs.exec_prefix);
    fprintf(stderr, "bindir:         %s\n", pmix_install_dirs.bindir);
    fprintf(stderr, "sbindir:        %s\n", pmix_install_dirs.sbindir);
    fprintf(stderr, "libexecdir:     %s\n", pmix_install_dirs.libexecdir);
    fprintf(stderr, "datarootdir:    %s\n", pmix_install_dirs.datarootdir);
    fprintf(stderr, "datadir:        %s\n", pmix_install_dirs.datadir);
    fprintf(stderr, "sysconfdir:     %s\n", pmix_install_dirs.sysconfdir);
    fprintf(stderr, "sharedstatedir: %s\n", pmix_install_dirs.sharedstatedir);
    fprintf(stderr, "localstatedir:  %s\n", pmix_install_dirs.localstatedir);
    fprintf(stderr, "libdir:         %s\n", pmix_install_dirs.libdir);
    fprintf(stderr, "includedir:     %s\n", pmix_install_dirs.includedir);
    fprintf(stderr, "infodir:        %s\n", pmix_install_dirs.infodir);
    fprintf(stderr, "mandir:         %s\n", pmix_install_dirs.mandir);
    fprintf(stderr, "pkgdatadir:     %s\n", pmix_install_dirs.pkgdatadir);
    fprintf(stderr, "pkglibdir:      %s\n", pmix_install_dirs.pkglibdir);
    fprintf(stderr, "pkgincludedir:  %s\n", pmix_install_dirs.pkgincludedir);
#endif

    /* NTH: Is it ok not to close the components? If not we can add a flag
       to mca_base_framework_components_close to indicate not to deregister
       variable groups */
    return PMIX_SUCCESS;
}


static int
pmix_installdirs_base_close(void)
{
    free(pmix_install_dirs.prefix);
    free(pmix_install_dirs.exec_prefix);
    free(pmix_install_dirs.bindir);
    free(pmix_install_dirs.sbindir);
    free(pmix_install_dirs.libexecdir);
    free(pmix_install_dirs.datarootdir);
    free(pmix_install_dirs.datadir);
    free(pmix_install_dirs.sysconfdir);
    free(pmix_install_dirs.sharedstatedir);
    free(pmix_install_dirs.localstatedir);
    free(pmix_install_dirs.libdir);
    free(pmix_install_dirs.includedir);
    free(pmix_install_dirs.infodir);
    free(pmix_install_dirs.mandir);
    free(pmix_install_dirs.pmixdatadir);
    free(pmix_install_dirs.pmixlibdir);
    free(pmix_install_dirs.pmixincludedir);
    memset (&pmix_install_dirs, 0, sizeof (pmix_install_dirs));

    return mca_base_framework_components_close (&pmix_installdirs_base_framework, NULL);
}

/* Declare the installdirs framework */
MCA_BASE_FRAMEWORK_DECLARE(pmix, installdirs, NULL, NULL, pmix_installdirs_base_open,
                           pmix_installdirs_base_close, mca_installdirs_base_static_components,
                           MCA_BASE_FRAMEWORK_FLAG_NOREGISTER | MCA_BASE_FRAMEWORK_FLAG_NO_DSO);
