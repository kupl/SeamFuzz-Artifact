#!/bin/bash -ex
# a running script for SeamFuzz artifact accpeted in ICSE'23
# As all experiments were evaluated on FuzzBench framework, 
# this script will run the fuzzbench framework with the given local experiment configurations, and produce the results of the main evaluation. 
# TRIALS: the number of trials of the intended experiments. The default value is 5
# TIME: the seconds for the running experiments. The default value is 86400, which is 24 hours
# [EXP_PATH]: the path for storing the running data of FuzzBench. The upper-case letter must not be included in the path. 
#             The default path is the working directory/data/
# [REPORT]: the path for storing the report of FuzzBench. The upper-case letter must not be included in the path.
#           The default path is the working directory/report/
# [EXP_PATH]: the name of the experiments which will be stored in [EXP_PATH]. The default name is maineval
# ./make_local_config.sh -t [TRIALS] -s [TIME] -p [EXP_PATH] -r [REPORT] -e [EXP_NAME]
# ex) ./main_eval.sh -t 10 -s 900 -p ~/8tb/frango/icse_data/ -r ~/8tb/frango/icse_report/ maineval5

SHELL_PATH=`pwd -P`
CONFIG="local-experiment-config.yaml"
FUZZBENCH_PATH="$SHELL_PATH/fuzzbench/"

TRIALS=20
TIME=86400
EXP_PATH="$SHELL_PATH/data/"
REPORT="$SHELL_PATH/report/"
EXP_NAME="maineval"

while getopts "t:s:p:r:e:" opt; do
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
        ?) 
            echo "Invalid options"
            ;;
    esac
done

CONFIG_CMD="-t $TRIALS -s $TIME -e $EXP_PATH -r $REPORT"

./make_local_config.sh $TRIALS $TIME $EXP_PATH $REPORT

cp ./local-experiment-config.yaml $FUZZBENCH_PATH/

# running fuzzbench with all benchmarks used in the main evaluation.
PYTHONPATH=$FUZZBENCH_PATH/ python3 $FUZZBENCH_PATH/experiment/run_experiment.py -cb 2 -a -c $FUZZBENCH_PATH/local-experiment-config.yaml -b arrow_parquet-arrow-fuzz grok_grk_decompress_fuzzer infotocap libarchive_libarchive_fuzzer zstd_stream_decompress libpng_test libxml2_magma objdump openssl_x509 magma_php podofo poppler_pdf_fuzzer proj4_standard_fuzzer sqlite3_ossfuzz -f aflpp seamfuzz aflppmopt -e $EXP_NAME


# Read the report data and print the results of the evaluation

# run extract_raw_data script to extract the results from the raw data produced by each fuzzers. (strongly related to the results of AFL++)
# It is to calculate the number of generated crash inputs by each fuzzer for all benchmarks
./extract_raw_data.sh $EXP_PATH $EXP_NAME

# run generate_table.py file to generate result tables from the experiments
python3.9 generate_table.py $REPORT $TIME $EXP_NAME

# print out the result table.
cat $SHELL_PATH/results/result_table
