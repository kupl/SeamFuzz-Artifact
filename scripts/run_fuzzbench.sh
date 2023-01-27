#!/bin/bash -ex
# a running script for SeamFuzz artifact accpeted in ICSE'23
# As all experiments were evaluated on FuzzBench framework, 
# this script will run the fuzzbench framework with the given local experiment configurations, and produce the results of the main evaluation. 
# TRIALS: the number of trials of the intended experiments. The default value is 20
# TIME: the seconds for the running experiments. The default value is 86400, which is 24 hours
# [EXP_PATH]: the path for storing the running data of FuzzBench. must be given as an absolute path. The default path is the working directory/data/
# [REPORT]: the path for storing the report of FuzzBench. must be given as an absolute path. The default path is the working directory/report/
# [EXP_PATH]: the name of the experiments which will be stored in [EXP_PATH]. The default name is maineval
# [FUZZBENCH_PATH]: the path for fuzzbench
# [BENCHMARK_PATH]: the "bench" file which specify benchmark programs for evaluation. "bench" file is in scripts directory
#
# ./run_fuzzbench.sh -t [TRIALS] -s [TIME] -p [EXP_PATH] -r [REPORT] -e [EXP_NAME] -F [FUZZBENCH_PATH] -b [BENCHMARK_PATH] -f [FUZZER_PATH]
# ex) ./run_fuzzbench.sh -t 10 -s 900 -p ~/data/ -r ~/report/ test -f ~/fuzzbench -b ./bench.txt -F ./fuzzbench

SHELL_PATH=`pwd -P`
CONFIG="local-experiment-config.yaml"
FUZZBENCH_PATH="$SHELL_PATH/fuzzbench/"
BENCHMARK_PATH="$SHELL_PATH/scripts/bench.txt"
FUZZER_PATH="$SHELL_PATH/scripts/fuzzer.txt"
BENCH=""
FUZZER=""

RESULTS="$SHELL_PATH/results"

TRIALS=5
TIME=86400
EXP_PATH="$SHELL_PATH/data/"
REPORT="$SHELL_PATH/report/"
EXP_NAME="test"

while getopts "t:s:p:r:e:F:b:f:" opt; do
    case $opt in
        t)
            TRIALS=$OPTARG
            ;;
        s)
            TIME=$OPTARG
            ;;
        p)
            EXP_PATH=$OPTARG
            ;;
        r)
            REPORT=$OPTARG
            ;;
        e)
            EXP_NAME=$OPTARG
            ;;
        F)
            FUZZBENCH_PATH=$OPTARG
            ;;
        b)
            BENCHMARK_PATH=$OPTARG
            ;;
        f)
            FUZZER_PATH=$OPTARG
            ;;
        ?) 
            echo "Invalid options"
            ;;
    esac
done

./make_local_config.sh $TRIALS $TIME $EXP_PATH $REPORT $FUZZBENCH_PATH

cp $SHELL_PATH/local-experiment-config.yaml $FUZZBENCH_PATH/ 

while read line || [ -n "$line" ] ; do
    BENCH="$BENCH $line"
done < $BENCHMARK_PATH


while read line || [ -n "$line" ] ; do
    FUZZER="$FUZZER $line"
done < $FUZZER_PATH

source $FUZZBENCH_PATH/.venv/bin/activate

# running fuzzbench with all benchmarks used in the main evaluation.
PYTHONPATH=$FUZZBENCH_PATH/ python3 $FUZZBENCH_PATH/experiment/run_experiment.py -cb 1 -a -c $FUZZBENCH_PATH/local-experiment-config.yaml -b $BENCH -f $FUZZER -e $EXP_NAME

deactivate

# Read the report data and print the results of the evaluation.
# run count_crash_inputs script to extract the results from the raw data produced by each fuzzers. (using the results-fuzzer_stats- of AFL++)
# It is to calculate the number of generated crash inputs by each fuzzer for all benchmarks
./count_crash_inputs.sh $EXP_PATH $EXP_NAME $RESULTS

# run generate_table.py file to generate result tables from the experiments
python3 generate_table.py $REPORT $EXP_PATH $TIME $TRIALS $EXP_NAME $RESULTS

# print out the result table.
cat $RESULTS/$EXP_NAME/result_table
