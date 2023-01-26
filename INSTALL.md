# Setup
All experiments in our paper was done on [FuzzBench](https://github.com/google/fuzzbench) framework,
and therefore, the setup environments for our artifact is the same as the one for [FuzzBench](https://github.com/google/fuzzbench).
Please refer to the following [link](https://google.github.io/fuzzbench/) to establish the experimental environments.
You can also follow the instructions below to construct/evaluate SeamFuzz on FuzzBench.

We modified some parts of FuzzBench to evaluate some benchmark programs (objdump, infotocap, podofopdfinfo, magma_libxml2_xmllint) as follows:
```
fuzzbench
└── requirements.txt : modify some parts of the requirements.txt to make it work on > python3.9
└── experiment
|      └── measurer
|             └── run_coverage.py : "do_coverage_run" function is modified to properly evaluate some benchmark programs
└── fuzzers
|      └── afl
|      |    └── fuzzer.py         : "run_afl_fuzz" function is modified to properly evaluate some benchmark programs
       |
       └── aflpp : "build.Dockerfile" is modified to make the version of "AFL++" be the same as the one of SeamFuzz
       |
       └── aflppmopt : "build.Dockerfile" is modified to make the version of "AFL++" be the same as the one of SeamFuzz
```
If the users want to evaluate our fuzzers on the FuzzBench framework from original git, 
please modify the files above.
Otherwise, FuzzBench may not properly work on some benchmark programs we provide.



### From Source
Following command will construct the experimental environments.  

```
cd ./fuzzbench && ./setup.sh
```


### VM(VirtualMachine) 
We also provide a VM image file which contain all contents to evaluate the experiments.
The VM image is built on [VirtualBox 7.0](https://www.virtualbox.org).
You can donwload the VM image from the following link.

The user password is set to "1234"
