# Structure
As SeamFuzz is built on [AFL++](https://github.com/aflplusplus/aflplusplus)-v3.15, it generally follows the same structure of AFL++.
In this document, we briefly explain the source codes we added/modified to AFL++ for SeamFuzz implementation.

```
SeamFuzz                           
├── include                 
│       └──  afl-fuzz.h         a header file for AFL++. We added components for SeamFuzz. 
│
├── src                 
│    ├── afl-fuzz-seam.c        A C source code which contains functions for SeamFuzz. 
│    │                          It contains the components for seed cluster, similarity calculation, and learning algorithm.
|    |
│    ├── afl-fuzz.c             We modified some parts of this source file to get SeamFuzz as options
│    |
│    ├── afl-fuzz-one.c         We added "fuzz_one_seam" function for running SeamFuzz as a module.
|    |
│    ├── afl-fuzz-queue.c       We modified "add_to_queue" function for storing covered path information
|    |                          of the newly added seed inputs.
|    | 
│    ├── afl-fuzz-init.c        Modified "perform_dry_run" to store the covered path information of the initial seed inputs
|    |
│    ├── afl-fuzz-run.c         We added "common_fuzz_stuff_seam" for tracking the performance of generated mutants.
|    |                          By observing the performance, SeamFuzz obtains the good/bad data for learning mutation strategies.
│    |   ...
│    └── ...
|
├── thompson.py                 a python file for computing thompson sampling algorithm. 
|      ...
└── ...

```
