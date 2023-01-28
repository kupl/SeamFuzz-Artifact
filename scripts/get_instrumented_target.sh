#!/bin/bash
# a running script for SeamFuzz artifact accpeted in ICSE'23
# As all experiments were evaluated on FuzzBench framework, 
# this script will use FuzzBench framework to obtain [FUZZER]-instrumented programs and the corresponding [FUZZER] binary file. 
# [FUZZBENCH] = an absolute path to FuzzBench framework (must be git cloned)
# [FUZZER]: [FUZZER] binary file
# [BENCHMARK]: [FUZZER]-instrumented benchmark programs
#
# ./get_instrumented_target.sh.sh [FUZZBENCH] [FUZZER] [BENCHMARK] 

SHELL_PATH=`pwd -P`
CONFIG="local-experiment-config.yaml"
INSTRUMENT_PATH=$SHELL_PATH/instrument
FUZZBENCH=${1}
FUZZER=${2}
BENCHMARK=${3}

CHECK=`docker images | grep 'builders' | grep $FUZZER | grep $BENCHMARK | grep -v 'intermediate' | head -1 | cut -d ' ' -f1`

CHECK_CONTAINER=`docker ps -a | grep 'get_container'`

# If no image have ever been built...
if [ $CHECK == "" ]; then	
	cd $FUZZBENCH && source .venv/bin/activate && make build-$FUZZER-$BENCHMARK && deactivate
fi

if [ ! "$CHECK_CONTAINER" == "" ]; then
	echo "HELLO"
	DELETE=`docker rm $(docker ps -a | grep 'get_container')`
fi


CONTAINER=`docker create --name='get_container' $CHECK`

mkdir -p $INSTRUMENT_PATH

docker cp get_container:/out/ $INSTRUMENT_PATH/$BENCHMARK

echo $DELETE
