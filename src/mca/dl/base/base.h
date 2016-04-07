/*
 * Copyright (c) 2004-2010 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2015 Cisco Systems, Inc.  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#ifndef OPAL_DL_BASE_H
#define OPAL_DL_BASE_H

#include "pmix_config.h"
#include "pmix/mca/dl/dl.h"
#include "pmix/util/pmix_environ.h"
#include "pmix/runtime/pmix_cr.h"

#include "pmix/mca/base/base.h"


BEGIN_C_DECLS

/**
 * Globals
 */
OPAL_DECLSPEC extern mca_base_framework_t pmix_dl_base_framework;
OPAL_DECLSPEC extern pmix_dl_base_component_t
*pmix_dl_base_selected_component;
OPAL_DECLSPEC extern pmix_dl_base_module_t *pmix_dl;


/**
 * Initialize the DL MCA framework
 *
 * @retval OPAL_SUCCESS Upon success
 * @retval OPAL_ERROR   Upon failures
 *
 * This function is invoked during pmix_init();
 */
OPAL_DECLSPEC int pmix_dl_base_open(mca_base_open_flag_t flags);

/**
 * Select an available component.
 *
 * @retval OPAL_SUCCESS Upon Success
 * @retval OPAL_NOT_FOUND If no component can be selected
 * @retval OPAL_ERROR Upon other failure
 *
 */
OPAL_DECLSPEC int pmix_dl_base_select(void);

/**
 * Finalize the DL MCA framework
 *
 * @retval OPAL_SUCCESS Upon success
 * @retval OPAL_ERROR   Upon failures
 *
 * This function is invoked during pmix_finalize();
 */
OPAL_DECLSPEC int pmix_dl_base_close(void);

/**
 * Open a DSO
 *
 * (see pmix_dl_base_module_open_ft_t in pmix/mca/dl/dl.h for
 * documentation of this function)
 */
OPAL_DECLSPEC int pmix_dl_open(const char *fname,
                               bool use_ext, bool private_namespace,
                               pmix_dl_handle_t **handle, char **err_msg);

/**
 * Lookup a symbol in a DSO
 *
 * (see pmix_dl_base_module_lookup_ft_t in pmix/mca/dl/dl.h for
 * documentation of this function)
 */
OPAL_DECLSPEC int pmix_dl_lookup(pmix_dl_handle_t *handle,
                                 const char *symbol,
                                 void **ptr, char **err_msg);

/**
 * Close a DSO
 *
 * (see pmix_dl_base_module_close_ft_t in pmix/mca/dl/dl.h for
 * documentation of this function)
 */
OPAL_DECLSPEC int pmix_dl_close(pmix_dl_handle_t *handle);

/**
 * Iterate over files in a path
 *
 * (see pmix_dl_base_module_foreachfile_ft_t in pmix/mca/dl/dl.h for
 * documentation of this function)
 */
OPAL_DECLSPEC int pmix_dl_foreachfile(const char *search_path,
                                      int (*cb_func)(const char *filename,
                                                     void *context),
                                      void *context);

END_C_DECLS

#endif /* OPAL_DL_BASE_H */
