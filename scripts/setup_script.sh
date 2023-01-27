#!/bin/bash -ex
# a running script for SeamFuzz artifact accpeted in ICSE'23
# As all experiments were evaluated on FuzzBench framework, 
# this script will build the fuzzbench framework for evaluation. 
# [FUZZBENCH_PATH]: the path for fuzzbench
#
# ./setup_script.sh [FUZZBENCH_PATH] 
# ex) ./setup_script.sh ./fuzzbench
FUZZBENCH_PATH=${1}

sudo apt-get install -y python3-venv python3-dev build-essential libpq-dev python3-pip docker-compose

sudo groupadd docker
sudo gpasswd -a $USER docker
newgrp docker

cd $FUZZBENCH_PATH && make

# build google-dispatcher-image which is compatible with the old version of FuzzBench (commit f1c1291)
docker pull audxo14/seam-dispatcher

docker create --name=get_dispatcher audxo14/seam-dispatcher:latest

docker cp get_dispatcher:/dispatcher.tar ./

docker rmi -f audxo14/seam-dispatcher

docker load -i ./dispatcher.tar 

