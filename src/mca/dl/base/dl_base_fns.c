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

/**
 * This file is a simple set of wrappers around the selected OPAL DL
 * component (it's a compile-time framework with, at most, a single
 * component; see dl.h for details).
 */

#include "pmix_config.h"

#include "pmix/include/pmix/constants.h"

#include "pmix/mca/dl/base/base.h"


int pmix_dl_open(const char *fname,
                 bool use_ext, bool private_namespace,
                 pmix_dl_handle_t **handle, char **err_msg)
{
    *handle = NULL;

    if (NULL != pmix_dl && NULL != pmix_dl->open) {
        return pmix_dl->open(fname, use_ext, private_namespace,
                             handle, err_msg);
    }

    return OPAL_ERR_NOT_SUPPORTED;
}

int pmix_dl_lookup(pmix_dl_handle_t *handle,
                   const char *symbol,
                   void **ptr, char **err_msg)
{
    if (NULL != pmix_dl && NULL != pmix_dl->lookup) {
        return pmix_dl->lookup(handle, symbol, ptr, err_msg);
    }

    return OPAL_ERR_NOT_SUPPORTED;
}

int pmix_dl_close(pmix_dl_handle_t *handle)
{
    if (NULL != pmix_dl && NULL != pmix_dl->close) {
        return pmix_dl->close(handle);
    }

    return OPAL_ERR_NOT_SUPPORTED;
}

int pmix_dl_foreachfile(const char *search_path,
                        int (*cb_func)(const char *filename, void *context),
                        void *context)
{
    if (NULL != pmix_dl && NULL != pmix_dl->foreachfile) {
        return pmix_dl->foreachfile(search_path, cb_func, context);
    }

    return OPAL_ERR_NOT_SUPPORTED;
}
