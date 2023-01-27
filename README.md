# SeamFuzz-Artifact
This repository is for the implementation of our paper accepted in ICSE 2023, "Learning Seed-Adaptive Mutation Strategies for
Greybox Fuzzing".
Our tool, [SeamFuzz](https://github.com/kupl/SeamFuzz-public), is a grey-box mutation-based fuzzer built on [AFL++](https://github.com/AFLplusplus/AFLplusplus)-v3.15++.

# Requirements
Please refer to [REQUIREMENTS.md](./REQUIREMENTS.md) for the operating system, hardware, and software requirements.

# Setup
Please refer to [INSTALL.md](./INSTALL.md) for installation.

# Structure
Please refer to [STRUCTURE.md](./STRUCTURE.md) for SeamFuzz structure description.

# Benchmarks
We offer all benchmarks used for our experiments and a script file for building each benchmarks in `benchmarks` directory.
Note that, we evaluated all benchmarks on FuzzBench framework.
To evaulate those benchmarks on FuzzBench framework, please refer to [benchmarks](./benchmarks).

If users prefer to run the fuzzers without using FuzzBench framework, you can follow the instructions written in `Dockerfile` and `build.sh` in each benchmark directory to build your own fuzzer implemented benchmark program.
We also offer a building script file `get_implemented_target.sh` which uses Fuzzbench Framework.
 
```
/SeamFuzz-Artifact$ ./scripts/get_implemented_target.sh [FUZZBENCH] [FUZZER] [BENCHMARK]

[FUZZBENCH]: an absolute path to FuzzBench Framework. 
[FUZZER] : any fuzzer in FuzzBench/fuzzers. 
[BENCHMARK] : any benchmark program in FuzzBench/benchmarks.
```

This script will check whether the benchmark-build Docker image generated by FuzzBench exists, and if not, it generates Docker image of the given [BENCHMARK] and [FUZZER].
After building Docker image is successfully done, it copies the [FUZZER]-implemented [BENCHMARK] programs in to `./implement` directory.

Note that [BENCHMARK] with proper Dockerfile and build scripts and [FUZZER] must be in `benchmarks` and `fuzzers` directories in users` FuzzBench framework.


# How to use
### Performing small experiments
We provide an example instruction which conducts a short experiment running **AFL++**, **AFL++_MOpt**, and **SeamFuzz** on a benchmark program libxml2-v2.9.2 with 2 trials during 3 hours. 
Note that conducting experiments for all benchmarks (Table 2 in our paper) takes at least **20,160 hours** (24 hours * 14 benchmarks * 20 trials * 3 fuzzers + N hours for building benchmarks/fuzzers) on a single core.

Once the setup instruction is successfully done(or using our VM image), you can perform the small experiments with the following command:

```
/SeamFuzz-Artifact& cat ./scripts/bench.txt
libxml2-v2.9.2

/SeamFuzz-Artifact$ ./scripts/main_eval.sh ./fuzzbench/ ./scripts/bench.txt 2 10800 test
```

Then, you will see the fuzzing progress, which is from FuzzBench, as follows:

```
...
INFO:root:Measuring all trials.
INFO:root:Measuring cycle : 9, Extras: {'fuzzer': 'aflpp', 'benchmark': 'libxml2-v2.9.2', 'trial_id':19', 'cycle':9}
INFO:root:Measuring cycle : 9, Extras: {'fuzzer': 'aflppmopt', 'benchmark': 'libxml2-v2.9.2', 'trial_id':20', 'cycle':9}
INFO:root:Measuring cycle : 9, Extras: {'fuzzer': 'seamfuzz', 'benchmark': 'libxml2-v2.9.2', 'trial_id':22', 'cycle':9}
...
```

When all process termintes, you can see the following output:
```
----------------------------------------------------------------------------------------------------------------------------
    program      |        AFL++       |                AFL++_MOpt               |                   SeamFuzz               |
                 |  Cover    Crashes  |  Cover    Crashes    R_Cov    R_Crashes |  Cover    Crashes    R_Cov    R_Crashes  |
----------------------------------------------------------------------------------------------------------------------------
libxml2-v2.9.2   |  3986        0     | 3274        0       -17.9%          0%  |  3997       0        0.3%            0%  |    
----------------------------------------------------------------------------------------------------------------------------
...
```
You can also check the result file in `./results/test/result_table.txt`

* Note that the result table above was produced by using our VM image.


### Performing full experiments.
We also provide running script files which reproduce all result tables(Table 2, 3, 4, and 5) in our paper.


Once the setup is properly prepared (or using VM), enter the following command to reproduce the main evlauation results in our paper, which is Table 2 in Section IV.

## With Reproducing-Script Files

### Table 2 & Figure 3

`main_eval.sh` will sequentially run FuzzBench with the given configurations and perform 3 script files: make_local_config.sh, extract_raw_data.sh, and generate_table.py.

```
./main_eval.sh -t [TRIALS] -s [TIME] -p [EXP_PATH] -r [REPORT] -e [EXP_NAME]

 [TRIALS]: the number of trials of the intended experiments. The default value is 20
 [TIME]: the seconds for the running experiments. The default value is 86400, which is 24 hours
 [EXP_PATH]: the path for storing the running data of FuzzBench. The default path is [the working directory]/data/
 [REPORT]: the path for storing the report of FuzzBench. The default path is [the working directory]/report/
 [EXP_NAME]: the name of the experiments which will be stored in [EXP_PATH]. The default name is maintable
```

The below description is for summarizing the goal of each script file.

```
make_local_config.sh: a script file to generate a local experiment configuration file(local-experiment-config.yaml) which is mandatory for running FuzzBench on a local machine.
extract_raw_data.sh: a script file which extracts the information of crash inputs generated by each fuzzer.
generate_table.py: a python file which generates the result table as in our paper.
```



Note that running FuzzBench requires lots of memory and storage. It may not work properly when running `main_eval.sh` script with the default settings. 
For example, running `main_eval.sh` without any additional options requires at least 840 (20 trials * 14 benchmarks * 3 fuzzers) CPU power. (The version we used FuzzBench assigns 1 CPU power to each experiment on a local machine)
If FuzzBench is eventually dead with `EOF` error, please set [TRIALS] option to a value less than 20 to reduce memory and storage usage. 

### Table N

```
./running_table[N].sh -t [TRIALS] -s [TIME] -p [EXP_PATH] -r [REPORT] -e [EXP_NAME]
```

`running_table[N].sh` will make a local experiment configuration file(`local-experiment-config.yaml`) for running FuzzBench on the local machine, running FuzzBench with the benchmark programs and fuzzers used in Table [N] in our paper, and generate result table for them.
For example, running_table3.sh will evaluate the experiments used to the Table 3 in our paper.

The options for `running_table[N].sh` is the same as the ones for `main_eval.sh`

### Result
After running each script file, `result` directory is produced in the current directory.
The details are as follows:

```
result
└── coverage.csv : contains average coverage of each fuzzer on each benchmark. (follows FuzzBench criteria)
|      
├── crashes.csv  : contains the number of crash inputs generated by each fuzzer on each benchmark (follows AFL++ criteria)
|      
├── result_table : contains a table which integrates coverage.csv and crashes.csv
|
└── unique_vul_venn_diagram.png : A Venn diagram for the unique vulnerabilities found by each tool (follows FuzzBench criteria)
```

## Without Reproducing-Script file 
You may want to run FuzzBench with your own flavors in usage.

```
./make_local_config.sh $TRIALS $TIME $EXP_PATH $REPORT
cp ./local-experiment-config.yaml [FUZZBENCH]

PYTHONPATH=[FUZZBENCH] \
python3 [FUZZBENCH]/experiment/run_experiment.py -cb [N] -a -c [FUZZBENCH]/local-experiment-config.yaml \
-b [BENCHMARKS] -f [FUZZERS] -e [EXP_NAME]
```

```
-cb [N]: cuncurrent building options. Higher the value, faster the building process. 
[BENCHMARKS] : give the full name of the the programs which will be used for experiments. The benchmark programs are in ./fuzzbench/benchmarks directory. ex) "-b arrow_parquet-arrow-fuzz grok_grk_decompress_fuzzer"
[FUZZERS] : give the full name of the fuzzers which will be used for experiments. The fuzzers are in ./fuzzbench/fuzzers directory. ex) "-f aflpp aflppmopt seamfuzz"
```

# Contact
Myungho Lee (e-mail: myungho_lee@korea.ac.kr)

