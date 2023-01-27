#!/bin/bash
# a running script for SeamFuzz artifact accpeted in ICSE'23
# As all experiments were evaluated on FuzzBench framework, 
# this script will make a local experiment configuration for running FuzzBench framework in a local setting. 
# TRIALS: the number of trials of the intended experiments. The default value is 20
# TIME: the seconds for the running experiments. The default value is 86400, which is 24 hours
# [EXP_PATH]: the path for storing the running data of FuzzBench. must be given as an absolute path. The default path is the working directory/data/
# [REPORT]: the path for storing the report of FuzzBench. must be given as an absolute path. The default path is the working directory/report/
#
# ./get_implemented_target.sh.sh [FUZZBENCH] [FUZZER] [BENCHMARK] 

SHELL_PATH=`pwd -P`
CONFIG="local-experiment-config.yaml"
IMPLEMENT_PATH=$SHELL_PATH/implement
FUZZBENCH=${1}
FUZZER=${2}
BENCHMARK=${3}

CHECK=`docker images | grep 'builders' | grep $FUZZER | grep $BENCHMARK | grep -v 'intermediate' | head -1 | cut -d ' ' -f1`

CHECK_CONTAINER=`docker ps -a | grep 'get_container'`

if [ ! "$CHECK_CONTAINER" == "" ]; then
	echo "HELLO"
	DELETE=`docker rm $(docker ps -a | grep 'get_container')`
fi


CONTAINER=`docker create --name='get_container' $CHECK`

mkdir -p $IMPLEMENT_PATH

docker cp get_container:/out/ ./implement/$BENCHMARK


