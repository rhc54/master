/*
 * Copyright (c) 2004-2010 The Trustees of Indiana University.
 *                         All rights reserved.
 * Copyright (c) 2015 Cisco Systems, Inc.  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "pmix_config.h"

#include "pmix/mca/mca.h"
#include "pmix/mca/base/base.h"

#include "pmix/mca/dl/dl.h"
#include "pmix/mca/dl/base/base.h"


int pmix_dl_base_close(void)
{
    /* Close all available modules that are open */
    return mca_base_framework_components_close(&pmix_dl_base_framework, NULL);
}
