
Handling Timeouts
=================

PMIx includes several collective operations (e.g., ``PMIx_Fence``, ``PMIx_Connect``, and ``PMIx_Group_construct``). Given that these are operations that require multiple processes to call the API before the operation can complete, there is always the potential that one or more processes will fail to participate - e.g., a process might hang in some intermediate step prior to reaching the point where the API would be called. This can result in an application blocking, thereby consuming the rest of its allocation time without performing any work.

PMIx provides a ``PMIX_TIMEOUT`` attribute to help protect against such errors. The intent of the attribute is to allow an application to specify the maximum time the requested operation should take - if the operation has not completed within the specified time limit, then PMIx is expected to terminate the operation and return a ``PMIX_ERR_TIMEOUT`` status.

Adding a timeout capability to a distributed operation inherently creates race conditions and raises questions over where and how the timeout is being monitored. Options for the latter include monitoring:

* at the local level (i.e., in the PMIx client library), where each process monitors its own timeout limit
* at the server level (i.e., in the PMIx server library), where the local server monitors the timeout limit from all participating local clients
* at the host environment level, where the host itself monitors an aggregated timeout limit based on participation from all involved processes

Each of these options creates its own challenges. The operational goals are to ensure that:

* the processes end the operation in a consistent state - i.e., that they either all see the operation as having failed, or all see it as having succeeded. Leaving the application in a confused state is considered an unacceptable result.
* the host environment is not left with *dangling* operations - i.e., with uncertainty over whether the contribution from a process is a late arrival from a preceding timed out operation or the start of a new operation.

This document will discuss some of the scenarios that highlight the challenges of meeting these goals, and use those to drive the design/tradeoff discussion over some potential solutions. For simplicity, let's consider the case where three processes (label them A, B, and C) are participating in a ``PMIx_Connect`` operation. We will look at both distributed timeout monitoring (i.e., where the timeout is detected at either the client or server level) and centralized timeout monitoring (i.e., where the host environment monitors for timeout).

.. note:: The PMIx Standard does not specify that all processes *must* pass the ``PMIX_TIMEOUT`` attribute collectively, or if individual processes may or may not pass it for the same operation. It also does not specify if the timeout values must be the same or can vary by process - and if the latter, then which value(s) are to be used. It does, however, require that timeout of the operation be a *global* result - i.e., that the entire operation must return a ``PMIX_ERR_TIMEOUT`` result to all participants if a timeout occurs.

          For the purposes of this discussion, we will assume the most flexible interpretation of the Standard. Thus, we will assume that some, but not all, participants provide a ``PMIX_TIMEOUT`` attribute, and that the timeout value may be unique to each participant that passes it. We will also assume that processes A and B are executing under the same PMIx server, while process C is a client of a different server.


Scenario 1: Incomplete
----------------------

In this scenario, processes A and B call ``PMIx_Connect`` in some time window that fits within their specified limits. However, process C (for whatever reason) never executes its call to the API. Thus, the operation times out for processes A and B once their specified time limit is reached.

Note that the local PMIx server will have notified the host of the operation once both processes A and B have participated in it. Thus, the host has knowledge of the operation and is waiting for process C to participate. Proper cleanup therefore requires that the host at least be notified that the timeout has occurred, and thus the operation has terminated.

This situation is amenable to both the distributed and centralized solution. If the individual clients or their local server are monitoring the timeout, then processes A and B can be notified without host involvement. However, the local server will need to notify the host that the operation is being cancelled so the host can clean up.

If the timeout responsibility has been delegated to the host, then the host can simply notify all participants that the operation has timed out. Note that process C cannot be notified as it will never participate in the operation, and therefore the ``PMIX_ERR_TIMEOUT`` status cannot be returned to it.

In the cases where either the host or the PMIx server is responsible for timeout monitoring, the question remains as to which timeout value to use - that from process A or process B? If the timeout values are the same, then obviously there is no issue. However, if the values are different, then some abiguity arises.

Arguments as to which one to use can be made for both options, and there is no obviously correct or wrong solution. Taking the smaller value puts a tighter constraint on the operation, and would be consistent with the assumption that the application depends on that tighter constraint. On the other hand, taking the larger value would make it more likely for the operation to succeed. It is up to the host environment to decide on its policy - probably something they should document so users know what to expect.

There are no identified race conditions in this scenario - so long as process C *never* calls the API.


Scenario 2: Late
----------------

In this scenario, processes A and B call `PMIx_Connect`` in some time window that fits within their specified limits. However, process C (for whatever reason) calls the API *after* process A's and process B's time limit expires - i.e., both processes A and B have reached their specified timeout limit prior to C calling the API.

First, consider the case where timeout is locally monitored by the PMIx client. Processes A and B have already timed out, and process C will time out of the requested operation if it eventually does call the API. The only issue here is that the host and local PMIx servers will have been told of the operation, and therefore need to be notified of the cancellation. Accordingly, the PMIx client library in this case is required to notify its server of the timeout, and the server must subsequently upcall its host to notify it as well. 

A similar situation emerges in the case where timeout is monitored by the PMIx server. 

Monitoring by the host relieves the race condition by putting timeout detection in a centralized location. 


 if it is being locally monitored (i.e., in the client itself). However, the host and/or the local PMIx server can be left in an uncertain state as they have no way of telling that the operation requested by C equates to the failed operation requested by A and B - e.g., it could be that the three processes are initiating a new ``PMIx_Connect`` operation across the three processes. Thus, the host and/or local PMIx server can be left with "dangling" operations waiting for completion, or can confuse process contributions from "old" operations with those from "new" operations.

Of the various PMIx collective operations, only those in the PMIx "Group" set directly involve passing a unique identifier in the API. However, while the PMIx Standard does require that the provided group ID string be unique within the user's session, it isn't clear that the ID cannot be reused. In other words, if an application constructs a group with ID "foo" and later destructs that group, it may be permissible to construct a new group with that same ID.

If we take that interpretation, then the late participation by process C could in fact be an initial participation in construction of a new group with the same ID as the one that previously failed. This creates a dilemma for the host and/or the local PMIx server that is difficult to resolve. While it may seem like an unlikely use-case, one can imagine it occurring more naturally in a fault tolerant environment where an application might retry an operation that failed - e.g., restart process C because it is running slow due to memory bloat, and then have all three processes retry the connect operation.

.. attention:: The PMIx library has adopted a policy of prohibiting reuse of group IDs to avoid confusion caused by late or delayed participants. The PMIx server maintains a list of "used" group IDs and will return a ``PMIX_ERR_USED_ID`` when it detects that a local client is attempting to construct a group with a previously used ID. However, this is limited to group IDs known to that specific server. Accordingly, we strongly advise that host environments maintain their own list of used group IDs, check new construct requests against it, and return the "used-id" error when reuse is detected.

Scenario 2: Delayed
-------------------

In this scenario, processes A and B call `PMIx_Connect`` in some time window that fits within their specified limits. However, process C (for whatever reason) calls the API *after* process A's time limit expires - but *before* process B's time limit expires.

For the case where the host is monitoring the time limit, this scenario defaults to that of Scenario 3 as only one time limit is being enforced. Process C will be considered "late" and the operation declared as "timed out" if the shorter time value is used, while the operation will be considered successful if the longer time value is used.

Unfortunately, this creates its own race condition - it is possible that the host has sent the "operation complete" notification, but the client hasn't received it before timing out. This leaves the host and client in an inconsistent situation, and can cause multiple clients to see an inconsistent result as some may receive the "operation complete" notification prior to timeout while others timeout before it arrives.




Scenario 4: Host Late
---------------------

In this scenario, all three processes ``PMIx_Connect`` within some time window that fits within their specified limits. However, the host environment (for whatever reason) responds to the completed operation *after* the time limit has expired. Note that the response will arrive at each process asynchronously due to transmission differences, and so some of the processes may receive the "operation complete" message *before* the specified timeout has expired, while others may receive it *after* the timeout expired.



