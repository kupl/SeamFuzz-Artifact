# SeamFuzz-Artifact
This repository is for the implementation of our paper accepted in ICSE 2023, "Learning Seed-Adaptive Mutation Strategies for
Greybox Fuzzing".
Our tool, [SeamFuzz](https://github.com/kupl/SeamFuzz-public), is a grey-box mutation-based fuzzer built on [AFL++](https://github.com/AFLplusplus/AFLplusplus).

# Setup
All experiments in our paper was done on [FuzzBench](https://github.com/google/fuzzbench) framework,
and therefore, the setup environments for our artifact is the same as the one for [FuzzBench](https://github.com/google/fuzzbench).
Please refer to the following [link](https://google.github.io/fuzzbench/) to establish the experimental environments.
You can also follow the instructions below to construct/evaluate SeamFuzz on FuzzBench.

### From Source
Following command will construct the experimental environments.  

```
cd ./fuzzbench && make
```


### VM(VirtualMachine) 
We also provide a VM image file which contain all contents to evaluate the experiments. 
You can donwload the VM image from the following link.

# How to use
Once the setup is properly prepared (or using VM), enter the following command to reproduce the main evlauation results in our paper, which is Table 2 in Section IV.


```
./main_eval.sh -t [TRIALS] -s [TIME] -p [EXP_PATH] -r [REPORT] -e [EXP_NAME]
```

`main_eval.sh` will make a local experiment configuration file(`local-experiment-config.yaml`) for running FuzzBench on the local machine, running FuzzBench with all benchmark programs and fuzzers we used in our paper, and generate result table for Table 2.

The options for `main_eval.sh` is for generating a local experiment configuration file, and the option descriptions are as follows:

```
 [TRIALS]: the number of trials of the intended experiments. The default value is 20
 [TIME]: the seconds for the running experiments. The default value is 86400, which is 24 hours
 [EXP_PATH]: the path for storing the running data of FuzzBench. The default path is [the working directory]/data/
 [REPORT]: the path for storing the report of FuzzBench. The default path is [the working directory]/report/
 [EXP_NAME]: the name of the experiments which will be stored in [EXP_PATH]. The default name is maintable
```

# Contact
Myungho Lee (e-mail: myungho_lee@korea.ac.kr)

