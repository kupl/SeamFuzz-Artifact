# Setup
## Building SeamFuzz for Fuzzbench
All experiments in our paper were done on [FuzzBench](https://github.com/google/fuzzbench) framework,
and therefore, the setup environments for our artifact is the same as the one for [FuzzBench](https://github.com/google/fuzzbench).
Please refer to the following [link](https://google.github.io/fuzzbench/) to establish the experimental environments.
As FuzzBench requires `docker`, please refer to [Docker](https://docs.docker.com/engine/install/linux-postinstall) for Docker installation.

You can also follow the instructions below to construct/evaluate SeamFuzz on FuzzBench.

We modified some parts of FuzzBench to evaluate some benchmark programs (objdump, infotocap, podofopdfinfo) as follows:
```
fuzzbench
└── experiment
|   └── measurer
|       └── run_coverage.py : "do_coverage_run" function is modified to properly evaluate some benchmark programs
└── fuzzers
|   └── afl
|   |   └── fuzzer.py         : "run_afl_fuzz" function is modified to properly evaluate some benchmark programs
|   └── aflpp : 
|   |   └── build.Dockerfile  : to make the version of "AFL++" be the same as the one of SeamFuzz
|   |   └── fuzzer.py         : "run_afl_fuzz" function is modified to properly evaluate some benchmark programs
|   └── aflppmopt : 
|   |   └── build.Dockerfile  : to make the version of "AFL++" be the same as the one of SeamFuzz
|   |   └── fuzzer.py         : "L" option is set to 1 to evaluate MOpt. It is the default option described in [MOpt](https://github.com/puppet-meteor/MOpt-AFL)
|   |  
...
```
If the users want to evaluate our fuzzers on the FuzzBench framework from original git, 
please modify the files above.
Otherwise, FuzzBench may not properly work on some benchmark programs we provide.

For more detail, please refer to [benchmarks](./benchmarks).

### Using VM(VirtualMachine) 
We also provide a VM image file which contain all contents to evaluate the experiments.
The VM image is built on [VirtualBox 7.0](https://www.virtualbox.org).
This VM is set to use 48GB memories and 8 CPU cores with 1.5 TB disk.
The sudo user password is set to "1234"
You can donwload the VM image from the following link.

## Building SeamFuzz without FuzzBench
The source code for SeamFuzz is on ./fuzzbench/fuzzers/seamfuzz/afl. 
As SeamFuzz is built on AFL++, the installation process for SeamFuzz is the same as AFL++.
Please follow the instructions in [AFL++](https://github.com/AFLplusplus/AFLplusplus) to build SeamFuzz on a local machine.
