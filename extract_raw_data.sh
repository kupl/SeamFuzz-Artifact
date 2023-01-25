#!/bin/bash
# a running script for SeamFuzz artifact accpeted in ICSE'23
# As all experiments were evaluated on FuzzBench framework, 
# this script will run the fuzzbench framework with the given local experiment configurations, and produce the results of the main evaluation. 
# [EXP_PATH]: the path for storing the running data of FuzzBench. The default path is the working directory/data/
# [EXP_PATH]: the name of the experiments which are stored in [EXP_PATH]. The default name is maineval
# ./extract_raw_data.sh [EXP_PATH] [EXP_NAME]

SHELL_PATH=`pwd -P`

crash_dir=$SHELL_PATH/results
crash_file=$crash_dir/crashes.csv

if [ ! -d $crash_dir ]; then
    mkdir $crash_dir
fi

if [ ! -e $crash_file ]; then
    touch $crash_file
    echo "experiment,fuzzer,benchmark,trial_id,crashes" > $crash_file
fi

for dir in ${1}/${2}/experiment-folders/*; do
    dir_name=${dir##/*/}
    bench=$(echo $dir_name | rev | cut -d '-' --fields=2- | rev )   # find the name of the benchmark
    fuzzer=$(echo $dir_name | rev | cut -d '-' --fields=1 | rev )   # find the name of the fuzzer

    echo "Processing $dir_name..."

    for trials in ${dir}/*; do
        trial_num=${trials##/*/}
        trial_num=$(echo $trial_num | cut -d '-' --fields=2)

        last_corpus=$(ls -v ${trials}/corpus/* | tail -1)           # find the last corpus tar file

        rm -rf $SHELL_PATH/extracted
        mkdir $SHELL_PATH/extracted

        tar -xf $last_corpus -C $SHELL_PATH/extracted/

        crash_num=$(grep 'unique_crashes' $SHELL_PATH/extracted/corpus/default/fuzzer_stats | sed 's/[^0-9]//g')    # find the number of crashes reported in fuzzer_stats, which is produced by AFL++.

        echo "${2},${fuzzer},${bench},${trial_num},$crash_num" >> $crash_file # store the crash information in crashes.csv file

    done    

done


echo "Done: Generate crashes.csv"