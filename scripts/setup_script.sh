#!/bin/bash -ex
# a running script for SeamFuzz artifact accpeted in ICSE'23
# As all experiments were evaluated on FuzzBench framework, 
# this script will build the fuzzbench framework for evaluation. 
# [FUZZBENCH_PATH]: the path for fuzzbench
# [FUZZBENCH_PATH]: the path for seamfuzz artifact repository
# ./setup_script.sh [FUZZBENCH_PATH] [SEAMFUZZ_PATH]
# ex) ./setup_script.sh ./fuzzbench ./seamfuzz-artifact
FUZZBENCH_PATH=${1}

pushd $FUZZBENCH_PATH
make
popd

# build google-dispatcher-image which is compatible with the old version of FuzzBench (commit f1c1291)
docker pull audxo14/seam-dispatcher

docker create --name=get_dispatcher audxo14/seam-dispatcher:latest

docker cp get_dispatcher:/dispatcher.tar ./

docker rmi -f audxo14/seam-dispatcher

docker load -i ./dispatcher.tar 

pushd $FUZZBENCH_PATH
source .venv/bin/activate && pip install -r requirements && pip install matplotlib_venn && deactivate
popd

mv $SEAMFUZZ_PATH/benchmarks/* $FUZZBENCH_PATH/benchmarks/
mv $SEAMFUZZ_PATH/fuzzers/* $FUZZBENCH_PATH/fuzzers/
