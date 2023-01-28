# SeamFuzz-Artifact
This repository is for the implementation of our paper accepted in ICSE 2023, "Learning Seed-Adaptive Mutation Strategies for
Greybox Fuzzing".
Our tool, SeamFuzz, is a grey-box mutation-based fuzzer built on [AFL++](https://github.com/AFLplusplus/AFLplusplus)-v3.15++.

[Learning Seed-Adaptive Mutation Strategies for Greybox Fuzzing](https://drive.google.com/file/d/122EviTKVZW45nuSksAPEYUUvDoYTGNmG/view?usp=share_link) : Accepted Version

# Requirements
Please refer to [REQUIREMENTS.md](./REQUIREMENTS.md) for the operating system, hardware, and software requirements.

# Setup
Please refer to [INSTALL.md](./INSTALL.md) for installation.

# Structure
Please refer to [STRUCTURE.md](./STRUCTURE.md) for SeamFuzz structure description.

# Benchmarks
We offer all benchmarks used for our experiments and a script file for building each benchmarks in `benchmarks` directory.
Note that, we evaluated all benchmarks on FuzzBench framework.
To evaulate those benchmarks on FuzzBench framework, copy all contents in `benchmarks` directory into the benchmark directory in users' FuzzBench framework.

We also offer a script file `get_instrumented_target.sh` which extracts the instrumented target programs and the fuzzer binary file from FuzzBench docker image.
 
```
/SeamFuzz-Artifact$ ./scripts/get_instrumented_target.sh [FUZZBENCH] [FUZZER] [BENCHMARK]

[FUZZBENCH]: a path to FuzzBench Framework. 
[FUZZER] : any fuzzer in FuzzBench/fuzzers. 
[BENCHMARK] : any benchmark program in FuzzBench/benchmarks.
```

This script will check whether the benchmark-build Docker image generated by FuzzBench exists, and if not, it generates Docker image of the given [BENCHMARK] and [FUZZER].
After building Docker image is successfully done, it copies the [FUZZER]-instrumented [BENCHMARK] programs in to `./instrument` directory.

For example, you can obtain AFL++ and AFL++-instrumented "objdump" on our VM image, run the command as follows:
```
/SeamFuzz-Artifact$ ./scripts/get_instrumented_target.sh ../fuzzbench aflpp objdump
/SeamFuzz-Aritfact$ ls ./instrument/objdump/
afl++.dict  afl-fuzz  cmplog  objdump  objdump_Cov  seeds  thompson.py
```

Note that [BENCHMARK] with proper Dockerfile and build scripts and [FUZZER] must be in `benchmarks` and `fuzzers` directories in users` FuzzBench framework.


# How to use
### Performing small experiments
We provide an example instruction which conducts a short experiment running **AFL++**, **AFL++_MOpt**, and **SeamFuzz** on a benchmark program libxml2-v2.9.2 with 2 trials during 3 hours. 
Note that conducting experiments for all benchmarks (Table 2 in our paper) takes at least **20,160 hours** (24 hours * 14 benchmarks * 20 trials * 3 fuzzers + N hours for building benchmarks/fuzzers) on a single core.

Once the setup instruction is successfully done(or using our VM image), you can perform the small experiments with the following command:

```
/SeamFuzz-Artifact$ echo "libxml2-v2.9.2" > ./scripts/bench.txt
/SeamFuzz-Artifact$ echo "aflpp aflppmopt seamfuzz" > ./scripts/fuzzer.txt
/SeamFuzz-Artifact$ source ./fuzzbench/.venv/bin/activate
/SeamFuzz-Artifact$ ./scripts/run_fuzzbench.sh -t 2 -s 10800 -p ~/data/ -r ~/report/ -F ./fuzzbench/ -b ./scripts/bench.txt -f ./scripts/fuzzer.txt -e libxml2test
```

Then, you will see the fuzzing progress, which is from FuzzBench, as follows:

```
...
INFO:root:Measuring all trials.
INFO:root:Measuring cycle : 9, Extras: {'fuzzer': 'aflpp', 'benchmark': 'libxml2-v2.9.2', 'trial_id':19', 'cycle':9}
INFO:root:Measuring cycle : 9, Extras: {'fuzzer': 'aflpp', 'benchmark': 'libxml2-v2.9.2', 'trial_id':20, 'cycle':9}
INFO:root:Measuring cycle : 9, Extras: {'fuzzer': 'aflppmopt', 'benchmark': 'libxml2-v2.9.2', 'trial_id':20', 'cycle':9}
INFO:root:Measuring cycle : 9, Extras: {'fuzzer': 'aflppmopt', 'benchmark': 'libxml2-v2.9.2', 'trial_id':21', 'cycle':9}
INFO:root:Measuring cycle : 9, Extras: {'fuzzer': 'seamfuzz', 'benchmark': 'libxml2-v2.9.2', 'trial_id':22', 'cycle':9}
INFO:root:Measuring cycle : 9, Extras: {'fuzzer': 'seamfuzz', 'benchmark': 'libxml2-v2.9.2', 'trial_id':23', 'cycle':9}
...
```

When all process termintes, you can see the following output:
```
Experiment  : libxml2test
Total Time  : 10800 seconds
Total Trials: 2 trials
----------------------------------------------------------------------------------------------------------------------------
    program      |        AFL++       |                AFL++_MOpt               |                   SeamFuzz               |
                 |  Cover    Crashes  |  Cover    Crashes    R_Cov    R_Crashes |  Cover    Crashes    R_Cov    R_Crashes  |
----------------------------------------------------------------------------------------------------------------------------
libxml2-v2.9.2   |  3986        0     | 3274        0       -17.9%          0%  |  3997       0        0.3%            0%  |    
----------------------------------------------------------------------------------------------------------------------------
...
```
You can also check the result file in `./results/test/result_table.txt`

You can evaluate other benchmark (e.g., objdump) with the similar commands/script files as below:

```
/SeamFuzz-Artifact$ echo "objdump" > ./scripts/bench.txt
/SeamFuzz-Artifact$ ./scripts/run_fuzzbench.sh -t 2 -s 10800 -p ~/data/ -r ~/report/ -F ./fuzzbench/ -b ./scripts/bench.txt -f ./scripts/fuzzer.txt -e objdumptest
...

Experiment  : objdumptest
Total Time  : 10800 seconds
Total Trials: 2 trials
----------------------------------------------------------------------------------------------------------------------------
    program      |        AFL++       |                AFL++_MOpt               |                   SeamFuzz               |
                 |  Cover    Crashes  |  Cover    Crashes    R_Cov    R_Crashes |  Cover    Crashes    R_Cov    R_Crashes  |
----------------------------------------------------------------------------------------------------------------------------
objdump          |   709        0     |   590       0       -16.8%          0%  |   732       0        3.2%            0%  |    
----------------------------------------------------------------------------------------------------------------------------
...
```

* Note that the instructions above were performed on our VM image.


### Performing full experiments.
By running `run_fuzzbench.sh` script file, users can do all experiments in our paper.

```
./run_fuzzbench.sh -t [TRIALS] -s [TIME] -p [EXP_PATH] -r [REPORT] -e [EXP_NAME] -F [FUZZBENCH_PATH] -b [BENCHMARK_PATH] -f [FUZZER_PATH] -T [TBL_TYPE]

 [TRIALS]: the number of trials of the intended experiments. The default value is 20
 [TIME]: the seconds for the running experiments. The default value is 86400, which is 24 hours
 [EXP_PATH]: the path for storing the running data of FuzzBench. The default path is [the working directory]/data/
 [REPORT]: the path for storing the report of FuzzBench. The default path is [the working directory]/report/
 [EXP_NAME]: the name of the experiments which will be stored in [EXP_PATH]. The default name is maintable
 [FUZZBENCH_PATH] : the path to fuzzbench. The default path is [working directory]/fuzzbench
 [BENCHMARK_PATH] : the path to bench.txt file which contains the benchmark programs to be evaluated. The default is [working_directory]/scripts/bench.txt
 [FUZZER_PATH] : the path to fuzzer.txt file which contains the fuzzers to be evaluated. The default is [working_directory]/scripts/fuzzer.txt
 [TBL_TYPE] : decides the types of the result table... default value is 1 which draws the table similar to Table 2 in our paper

```

The commands below performs experiments in our paper.

<details>
<summary> Table 2 </summary>
<div markdown="1">

```bash
 # Set the benchmark programs to evaluate 
 $ echo "arrow_parquet-arrow-fuzz grok_grk_decompress_fuzzer infotocap libarchive_libarchive_fuzzer zstd_stream_decompress libpng libxml2-v2.9.2 objdump openssl_x509 php_php-fuzz-parser podofo poppler_pdf_fuzzer proj4_standard_fuzzer sqlite3_ossfuzz" > ./scripts/bench.txt

 # Set fuzzers to evaluate
 $ echo "aflpp aflppmopt seamfuzz" > ./scripts/fuzzer.txt

 # Running scripts with 20 trials for 24 hours.
 $ ./scripts/run_fuzzbench.sh -t 20 -s 86400 -p ~/data/ -r ~/report/ -F ./fuzzbench/ -b ./scripts/bench.txt -f ./scripts/fuzzer.txt -e table2
```

</div>
</details>


<details>
<summary> Table 3 </summary>
<div markdown="1">

```bash
 # Set the benchmark programs to evaluate 
 $ echo "arrow_parquet-arrow-fuzz grok_grk_decompress_fuzzer infotocap libarchive_libarchive_fuzzer zstd_stream_decompress libpng libxml2-v2.9.2 objdump openssl_x509 php_php-fuzz-parser podofo poppler_pdf_fuzzer proj4_standard_fuzzer sqlite3_ossfuzz" > ./scripts/bench.txt

 # Set fuzzers to evaluate
 $ echo "nocluster eachcluster seamfuzz" > ./scripts/fuzzer.txt

 # Running scripts with 20 trials for 24 hours.
 $ ./scripts/run_fuzzbench.sh -t 20 -s 86400 -p ~/data/ -r ~/report/ -F ./fuzzbench/ -b ./scripts/bench.txt -f ./scripts/fuzzer.txt -e table3 -T 0
```

</div>
</details>


<details>
<summary> Table 4 </summary>
<div markdown="1">

```bash
 # Set the benchmark programs to evaluate 
 $ echo "arrow_parquet-arrow-fuzz grok_grk_decompress_fuzzer libxml2-v2.9.2 php_php-fuzz-parser objdump openssl_x509 poppler_pdf_fuzzer proj4_standard_fuzzer sqlite3_ossfuzz" > ./scripts/bench.txt

 # Set fuzzers to evaluate
 $ echo "naiveseamfuzz seamfuzz" > ./scripts/fuzzer.txt

 # Running scripts with 20 trials for 24 hours.
 $ ./scripts/run_fuzzbench.sh -t 20 -s 86400 -p ~/data/ -r ~/report/ -F ./fuzzbench/ -b ./scripts/bench.txt -f ./scripts/fuzzer.txt -e table4
```

</div>
</details>


<details>
<summary> Table 5 </summary>
<div markdown="1">

```bash
 # Set the benchmark programs to evaluate 
 $ echo "arrow_parquet-arrow-fuzz grok_grk_decompress_fuzzer libxml2-v2.9.2 php_php-fuzz-parser objdump openssl_x509 poppler_pdf_fuzzer proj4_standard_fuzzer sqlite3_ossfuzz" > ./scripts/bench.txt

 # Set fuzzers to evaluate
 $ echo "seamfuzz seamfuzz1 seamfuzz5 seamfuzz20" > ./scripts/fuzzer.txt

 # Running scripts with 20 trials for 24 hours.
 $ ./scripts/run_fuzzbench.sh -t 20 -s 86400 -p ~/data/ -r ~/report/ -F ./fuzzbench/ -b ./scripts/bench.txt -f ./scripts/fuzzer.txt -e table5 -T 0
```

</div>
</details>


* Note that running the command above may not work properly due to memory/CPU issues. In this case, please reduce the number of trials (-t options) or evaluate some parts of the benchmarks (reducing the number of benchmarks evaluating at one time).

### Result
After running `run_fuzzbench.sh` script file, `result` directory is produced in the current directory.
The details are as follows:

```
results
└── [EXP_NAME] : contains average coverage of each fuzzer on each benchmark. (follows FuzzBench criteria)
|   └── coverage.csv : contains the average value of coverage for each fuzzer on each benchmark (follows FuzzBench criteria)
|   ├── crashes.csv  : contains the number of crash inputs generated by each fuzzer on each benchmark (follows AFL++ criteria)
|   ├── result_table.txt : contains a table which integrates coverage.csv and crashes.csv
|   └── unique_vul_venn_diagram.png : A Venn diagram for the unique vulnerabilities found by each tool (follows FuzzBench criteria)
|
└── [EXP_NAME]
|
...
```

## Without running the given script file 

### On FuzzBench
You may want to run FuzzBench with your own flavors in usage.

```bash
 # Set local configuration 
 $ ./make_local_config.sh [TRIALS] [TIME] [EXP_PATH] [REPORT] [FUZZBENCH_PATH]

 $ source [FUZZBENCH_PATH]/.venv/bin/activate
 $ PYTHONPATH=[FUZZBENCH] \
 $ python3 [FUZZBENCH]/experiment/run_experiment.py -cb [N] -a -c [FUZZBENCH]/local-experiment-config.yaml -b [BENCHMARKS] -f [FUZZERS] -e [EXP_NAME]
 
 
-cb [N]: cuncurrent building options. Higher the value, faster the building process (may output EOF error if the value is too high for Hardware)
[BENCHMARKS] : give the full name of the the programs which will be used for experiments. The benchmark programs are in ./fuzzbench/benchmarks directory. ex) "-b arrow_parquet-arrow-fuzz grok_grk_decompress_fuzzer"
[FUZZERS] : give the full name of the fuzzers which will be used for experiments. The fuzzers are in ./fuzzbench/fuzzers directory. ex) "-f aflpp aflppmopt seamfuzz"
```

### Not on FuzzBench
As SeamFuzz is built on AFL++, the usage is exactly the same as AFL++. Please refer to [AFL++](https://github.com/AFLplusplus/AFLplusplus).
You can check the options of SeamFuzz and their meanings with the following command:

```bash
 # Complete build SeamFuzz from fuzzers/seamfuzz/afl/
 $ ./afl-fuzz -h 
 
 ...
 SeamFuzz settings:
  -K [1, 2]     - enable SeamFuzz
                  1: use the original SeamFuzz which utilizes SeamFuzz data classification
                  2: does not use SeamFuzz data classification (follows AFL++ criteria)
  -A path       - a path to thompson.py and enables using thompson sampling algorithm 
  -a float      - Set the value for alpha (syntactic similarity), default value is 0.1 [0 ... 1.0]
  -g float      - Set the value for gamma (cluster), default value is 0.8 [0 ... 1.0]
  -X [0, 100]   - Set the probability for exploration
  -u            - Set the updating value for rewards
 ...
```

Running AFL++ with SeamFuzz options to test `objdump` (assume that objdump is AFL++-instrumented) is as follows:

```bash
 # Assume that objdump is AFL++-implemented. 
 $ ./afl-fuzz -K 1 -A [path_to_thompson.py] -i [path_to_seeds] -o ./output -- ./objdump --dwarf-check -C -g -f --dwarf -x @@
 
 # Test objdump with setting alpha value to 0.5. 
 $ ./afl-fuzz -K 1 -A [path_to_thompson.py] -a 0.5 -i [path_to_seeds] -o ./output -- ./objdump --dwarf-check -C -g -f --dwarf -x @@
```

# Contact
Myungho Lee (e-mail: myungho_lee@korea.ac.kr)

