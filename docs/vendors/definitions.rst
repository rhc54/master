Definitions
===========

In this chapter we describe some common terms and conventions used throughout
the guide. PMIx has adopted the widespread use of
key-value ``attributes`` to add flexibility to the functionality expressed
in the APIs. Accordingly, the PMIx Standard has chosen to require that
the definition of each standard API include the passing of an array of
attributes. These provide a means of customizing the behavior of the API
as future needs emerge without having to alter or create new variants of it. In
addition, attributes provide a mechanism by which researchers can easily
explore new approaches to a given operation without having to modify the
API itself.

In an effort to maintain long-term backward compatibility, PMIx does not include large numbers of APIs that each focus on a narrow scope of functionality, but instead relies on the definition of fewer generic APIs that include arrays of key-value attributes for ``tuning`` the function's behavior. Thus, modifications to the PMIx standard primarily consist of the definition of new attributes along with a description of the APIs to which they relate and the expected behavior when used with those APIs.

Terminology
-----------

The following terminology is used throughout the guide:

* ``system management stack`` (aka. "SMS") is the software associated with the management of the overall system and execution of applications on that system

* ``scheduler`` refers to the component of the SMS responsible for scheduling of resource allocations. This is also generally referred to as the ``system workflow manager`` - for the purposes of this document, the ``WLM`` acronym will be used interchangeably to refer to the scheduler.

* ``resource manager`` (aka. "RM") is used in a generic sense to represent the subsystem that will host the PMIx server library. This could be a vendor-supplied resource manager or a third-party agent such as a programming model's runtime library.

* ``host environment`` is used interchangeably with "resource manager" to refer to the process hosting the PMIx server library.


* ``session`` refers to a set of resources assigned by the WLM that has been reserved for one or more users. A session is identified by a ``session ID`` that is unique within the scope of the governing WLM. Historically, HPC sessions have consisted of a static allocation of resources - i.e., a block of resources assigned to a user in response to a specific request and managed as a unified collection. However, this is changing in response to the growing use of dynamic programming models that require on-the-fly allocation and release of system resources. Accordingly, the term ``session`` in this document refers to a potentially dynamic entity, perhaps comprised of resources accumulated as a result of multiple allocation requests that are managed as a single unit by the WLM.

* ``job`` refers to a set of one or more ``applications`` executed as a single invocation by the user within a session with a unique identifier, the ``job ID``, assigned by the RM or launcher. For example, the command line `mpiexec -n 1 app1 : -n 2 app2` generates a single MPMD job containing two applications. A user may execute multiple jobs within a given session, either sequentially or concurrently.

* ``namespace`` refers to a character string value assigned by the RM to a job.  All applications executed as part of that job share the same namespace. The namespace assigned to each job must be unique within the scope of the governing WLM and often is implemented as a string representation of the numerical job ID. The namespace and job terms will be used interchangeably throughout the guide.

* ``application`` represents a set of identical, but not necessarily unique, execution contexts within a job.

* ``process`` is assumed for ease of presentation to be an operating system process, also commonly referred to as a ``heavyweight`` process. A process is often comprised of multiple ``lightweight threads``, commonly known as simply ``threads``.

* ``client`` refers to a process that was registered with the PMIx server prior to being started, and connects to that PMIx server via the ``PMIx_Init`` API using its assigned namespace and rank with the information required to connect to that server being provided to the process at time of start of execution.

* ``tool`` refers to a process that may or may not have been registered with the PMIx server prior to being started and intializes using the ``PMIx_tool_init`` API.

* ``clone`` refers to a process that was directly started by a PMIx client (e.g., using `fork/exec`) and calls ``PMIx_Init``, thus connecting to its local PMIx server using the same namespace and rank as its parent process.

* ``rank`` refers to the numerical location (starting from zero) of a process within the defined scope. Thus, ``job rank`` is the rank of a process within its job and is synonymous with its unqualified rank, while ``application rank`` is the rank of that process within its application.

* ``peer`` refers to another process within the same job.

* ``workflow`` refers to an orchestrated execution plan typically involving multiple jobs carried out under the control of a `workflow manager`. An example workflow might first execute a computational job to generate the flow of liquid through a complex cavity, followed by a visualization job that takes the output of the first job as its input to produce an image output.

* ``node`` refers to a single operating system instance. Note that this may encompass one or more physical objects.

* ``package`` refers to a single object that is either soldered or connected to a printed circuit board via a mechanical socket. Packages may contain multiple chips that include (but are not limited to) processing units, memory, and peripheral interfaces.

* ``processing unit`` (aka. "PU"), is the electronic circuitry within a computer that executes instructions. Depending upon architecture and configuration settings, it may consist of a single hardware thread or multiple hardware threads collectively organized as a `core`.

* ``fabric`` is used in a generic sense to refer to the networks within the system regardless of speed or protocol. Any use of the term `network` in the guide should be considered interchangeable with `fabric`.

* ``fabric device}`` refers to an operating system fabric interface, which may be physical or virtual. Any use of the term `NIC` in the document should be considered interchangeable with `fabric device`.

* ``fabric plane`` refers to a collection of fabric devices in a common logical or physical configuration. Fabric planes are often implemented in HPC clusters as separate overlay or physical networks controlled by a dedicated fabric manager.

* ``attribute`` refers to a key-value pair comprised of a string key (represented by a `pmix_key_t` structure) and an associated value containing a PMIx data type (e.g., boolean, integer, or a more complex PMIx structure). Attributes are used both as directives when passed as qualifiers to APIs (e.g., in a `pmix_info_t` array), and to identify the contents of information (e.g., to specify that the contents of the corresponding `pmix_value_t` in a `pmix_info_t}`represent the ``PMIX_UNIV_SIZE``).

* ``key`` refers to the string component of a defined `attribute`. The guide will often refer to passing of a `key` to an API (e.g., to the ``PMIx_Query_info`` or \``PMIx_Get`` APIs) as a means of identifying requested information. In this context, the `data type` specified in the attribute's definition indicates the data type the caller should expect to receive in return. Note that not all attributes can be used as keys as some have specific uses solely as API qualifiers.

* ``instant on`` refers to a PMIx concept defined as: "All information required for setup and communication (including the address vector of endpoints for every process) is available to each process at start of execution"


Naming Conventions
------------------

PMIx has adopted the following conventions:

* PMIx constants and attributes are prefixed with ``PMIX_``.

* Structures and type definitions are prefixed with ``pmix_``.

* The string representation of attributes are prefixed with ``pmix``.

* Underscores are used to separate words in a function or variable name.

* Lowercase letters are used in PMIx client APIs except for the PMIx prefix (noted below) and the first letter of the word following it. For example, ``PMIx_Get_version``.

* PMIx server and tool APIs are all lower case letters following the prefix - e.g., ``PMIx_server_register_nspace``.

* The ``PMIx_`` prefix is used to denote functions.

* The ``pmix_`` prefix is used to denote function pointer and type definitions.

Vendors should not use the ``PMIX_``, ``PMIx_``, or ``pmix_`` prefixes for symbols in their code so as to avoid symbol conflicts with the PMIx library.


Callback Conventions
--------------------

Many PMIx interfaces, particularly nonblocking interfaces, use a `void*` callback data object passed to the function that is then passed to the associated callback. On the client side, the callback data object is an opaque, client-provided context that the client can pass to a non-blocking call. When the nonblocking call completes, the callback data object is passed back to the client without modification by the PMIx library, thus allowing the client to associate a context with that callback. This is useful if there are many outstanding nonblocking calls.

A similar model is used for the server module functions. In this case, the PMIx library is making an upcall into its host via a PMIx server module callback function and passing a specific callback function pointer and callback data object. The library expects the host to call the `cbfunc` with the necessary arguments and pass back the original callback data obect upon completing the operation. This gives the server-side PMIx library the ability to associate a context with the callback (since multiple operations may be outstanding). The host has no visibility into the contents of the callback data object object, nor is permitted to alter it in any way.
