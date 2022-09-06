/*
 * Copyright (c) 2022      Nanook Consulting.  All rights reserved.
 *
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/types.h>
#include <unistd.h>

#include <pmix.h>

/* This example program is designed to mimic the MPI
 * connect/accept procedure, executed through use of
 * the PMIx_Group interfaces.
 */

int main(int argc, char *argv[])
{
    pmix_status_t rc;
    int size;
    pid_t pid;
    pmix_proc_t myproc;
    pmix_proc_t proc, parent;
    pmix_app_t app;
    pmix_proc_t peers[2];
    char hostname[1024];
    pmix_value_t *val = NULL;
    pmix_nspace_t nspace;

    pid = getpid();
    gethostname(hostname, 1024);

    /* the "port" string is given to us */
    if (2 < argc) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    port = argv[1];

    rc = PMIx_Init(&myproc, NULL, 0);
    if (PMIX_SUCCESS != rc) {
        fprintf(stderr, "Client: PMIx_Init failed: %s\n", PMIx_Error_string(rc));
        exit(1);
    }

    /* get our job size */
    PMIX_LOAD_PROCID(&proc, myproc.nspace, PMIX_RANK_WILDCARD);
    rc = PMIx_Get(&proc, PMIX_JOB_SIZE, NULL, 0, &val);
    if (PMIX_SUCCESS != rc) {
        fprintf(stderr, "Client ns %s rank %d: PMIx_Get job size failed: %s\n", myproc.nspace,
                myproc.rank, PMIx_Error_string(rc));
        goto done;
    }
    PMIX_VALUE_GET_NUMBER(rc, val, size, int);
    if (PMIX_SUCCESS != rc) {
        fprintf(stderr, "Client ns %s rank %d: get size number failed: %s\n", myproc.nspace,
                myproc.rank, PMIx_Error_string(rc));
        goto done;
    }
    PMIX_VALUE_RELEASE(val);

    rc = PMIx_Get(&myproc, PMIX_PARENT_ID, NULL, 0, &val);
    /* If we don't find it, then we're the parent - i.e., the
     * job that will start the other job so we can try
     * connect/accept procedure */
    if (PMIX_SUCCESS != rc) {
        if (0 == myproc.rank) {

            PMIX_APP_CONSTRUCT(&app);
            app.cmd = strdup(argv[0]);
            PMIX_ARGV_APPEND(rc, app.argv, argv[0]);
            app.maxprocs = 3;
            rc = PMIx_Spawn(NULL, 0, &app, 1, nspace);
            if (PMIX_SUCCESS != rc) {
                printf("Child failed to spawn\n");
                return rc;
            }
            /* in the most general case of MPI connect/accept, only
             * the root process on each side knows the port, so the
             * first step in the operation is an exchange between
             * the two root processes that includes the participants
             * from each side. PMIx, however, requires that all participants
             * call PMIx_Group_construct with the same array of
             * participants. We cannot do that here as the two sides
             * don't know the participants of the other side.
             *
             * So we begin by having the root of each side call
             * PMIx_Group_construct using the "port" as the group
             * ID, and including a list of participants from their
             * side of the operation. */

            grpinfo = PMIx_Info_list_start();
            /* we want a new Context ID (CID) assigned to this group */
            rc = PMIx_Info_list_add(grpinfo, PMIX_GROUP_ASSIGN_CONTEXT_ID, NULL, PMIX_BOOL);
            if (PMIX_SUCCESS != rc) {
                fprintf(stderr, "Client ns %s rank %d: PMIx_Info_list_add failed: %s\n",
                        myproc.nspace, myproc.rank, PMIx_Error_string(rc));
                goto done;
            }
            /* add in the IDs of the other participants from our side - in this example,
             * we will use ranks 1 and 3 */
            PMIX_DATA_ARRAY_CONSTRUCT(&darray, PMIX_PROC, 2);
            procs = (pmix_proc_t*)darray.array;
            PMIX_LOAD_PROCID(&procs[0], myproc.nspace, 1);
            PMIX_LOAD_PROCID(&procs[1], myproc.nspace, 3);
            rc = PMIx_Info_list_add(grpinfo, PMIX_GROUP_ADD_MEMBERS, &darray, PMIX_DATA_ARRAY);
            if (PMIX_SUCCESS != rc) {
                fprintf(stderr, "Client ns %s rank %d: PMIx_Info_list_add failed: %s\n",
                        myproc.nspace, myproc.rank, PMIx_Error_string(rc));
                goto done;
            }
            PMIX_DATA_ARRAY_DESTRUCT(&darray);

            /* convert the info list */
            rc = PMIx_Info_list_convert(grpinfo, &darray);
            if (PMIX_SUCCESS != rc) {
                fprintf(stderr, "Client ns %s rank %d: PMIx_Info_list_convert failed: %s\n",
                        myproc.nspace, myproc.rank, PMIx_Error_string(rc));
                goto done;
            }
            info = (pmix_info_t*)darray.array;
            ninfo = darray.size;
            PMIx_Info_list_release(grpinfo);

            /* construct the group */
            PMIX_PROC_CREATE(procs, 2);
            PMIX_LOAD_PROCID(&procs[0], myproc.nspace, myproc.rank);
            PMIX_LOAD_PROCID(&procs[1], nspace, 0);
            rc = PMIx_Group_construct(port, procs, 2, info, ninfo, &results, &nresults);
            PMIX_DATA_ARRAY_DESTRUCT(&darray);
            free(procs);
            if (PMIX_SUCCESS != rc) {
                fprintf(stderr, "Client ns %s rank %d: PMIx_Group_construct failed: %s\n",
                        myproc.nspace, myproc.rank, PMIx_Error_string(rc));
                goto done;
            }

            /* we should have two results objects - one containing the final
             * group membership and the other containing the assigned CID */
            if (NULL != results) {
                for (n=0; n < nresults; n++) {
                    if (PMIX_CHECK_KEY(&results[n], PMIX_GROUP_CONTEXT_ID)) {
                        cid = 0;
                        PMIX_VALUE_GET_NUMBER(rc, &results[0].value, cid, size_t);
                        fprintf(stderr, "Rank %d Group construct complete with status %s KEY %s CID %lu\n",
                                myproc.rank, PMIx_Error_string(rc), results[0].key, (unsigned long) cid);
                    } else if (PMIX_CHECK_KEY(&results[n], PMIX_GROUP_MEMBERSHIP)) {
                        procs = (pmix_proc_t*)results[n].value.data.darray->array;
                        nprocs = results[n].value.data.darray->size;
                        fprintf(stderr, "Rank %d Group construct complete with %lu members\n",
                                myproc.rank, (unsigned long) nprocs);
                    }
                }
            } else {
                fprintf(stderr, "Rank %d Group construct complete, but nothing returned\n", myproc.rank);
            }
        } else {
            /* if we are not the root, then we have to join the group a little differently.
             * We do NOT necessarily know the port, but we do know that our root process
             * will have included us via the ADD_MEMBERS attribute, so we should expect
             * to receive a group "invite" to join. Setup to receive it */
        }
            /* we now have the full membership AND the CID for this group - the latter
             * corresponds to the CID of the communicator for the new collection. This
             * completes the connect/accept operation so far as PMIx is concerned. The
             * remaining work involves creation and population of the MPI communicator
             * object and "activating" it */
        goto done;
    }

    /* Otherwise, we're the child */
    printf("Hello from the child %s.%u of %d on host %s pid %ld\n",
           myproc.nspace, myproc.rank, size, hostname, (long)pid);

done:
    PMIx_Finalize(NULL, 0);
    fprintf(stderr, "%d: exiting\n", pid);
    return 0;
}
