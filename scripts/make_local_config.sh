#!/bin/bash -ex
# a running script for SeamFuzz artifact accpeted in ICSE'23
# As all experiments were evaluated on FuzzBench framework, 
# this script will make a local experiment configuration for running FuzzBench framework in a local setting. 
# TRIALS: the number of trials of the intended experiments. The default value is 20
# TIME: the seconds for the running experiments. The default value is 86400, which is 24 hours
# [EXP_PATH]: the path for storing the running data of FuzzBench. must be given as an absolute path. The default path is the working directory/data/
# [REPORT]: the path for storing the report of FuzzBench. must be given as an absolute path. The default path is the working directory/report/
# [FUZZBENCH_PATH]: the path to fuzzbench
# ./create_local_config.sh [TRIALS] [TIME] [EXP_PATH] [REPORT] [FUZZBENCH_PATH]

CONFIG="local-experiment-config.yaml"

TRIALS=${1}
TIME=${2}
EXP_PATH=${3}
REPORT=${4}
FUZZBENCH_PATH=${5}

echo "trials: $TRIALS" > $FUZZBENCH_PATH/$CONFIG
echo "max_total_time: $TIME" >> $FUZZBENCH_PATH/$CONFIG
echo "docker_registry: gcr.io/fuzzbench" >> $FUZZBENCH_PATH/$CONFIG
echo "experiment_filestore: $EXP_PATH" >> $FUZZBENCH_PATH/$CONFIG
echo "report_filestore: $REPORT" >> $FUZZBENCH_PATH/$CONFIG
echo "local_experiment: true" >> $FUZZBENCH_PATH/$CONFIG
#echo "git_hash: \"git-hash\"" >> $FUZZBENCH_PATH/$CONFIG
