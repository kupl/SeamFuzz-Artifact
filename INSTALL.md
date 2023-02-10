# Setup
## Building SeamFuzz for Fuzzbench
All experiments in our paper were done on [FuzzBench](https://github.com/google/fuzzbench) framework,
and therefore, the setup environments for our artifact is the same as the one for [FuzzBench](https://github.com/google/fuzzbench).
Please refer to the following [link](https://google.github.io/fuzzbench/) to establish the experimental environments.
As FuzzBench requires `docker`, please refer to [Docker](https://docs.docker.com/engine/install/linux-postinstall) for Docker installation.

You can also follow the instructions below to construct/evaluate SeamFuzz on FuzzBench.

We modified some parts of FuzzBench to evaluate some benchmark programs (objdump, infotocap, podofopdfinfo) as follows:
```
fuzzbench
└── experiment
|   └── measurer
|       └── run_coverage.py : "do_coverage_run" function is modified to properly evaluate some benchmark programs
└── fuzzers
|   └── afl
|   |   └── fuzzer.py         : "run_afl_fuzz" function is modified to properly evaluate some benchmark programs
|   └── aflpp : 
|   |   └── build.Dockerfile  : to make the version of "AFL++" be the same as the one of SeamFuzz
|   |   └── fuzzer.py         : "run_afl_fuzz" function is modified to properly evaluate some benchmark programs.
|   └── aflppmopt : 
|   |   └── build.Dockerfile  : to make the version of "AFL++" be the same as the one of SeamFuzz
|   |   └── fuzzer.py         : "L" option is set to 1 to evaluate MOpt. It is the default option described in MOpt
|   |                           github(https://github.com/puppet-meteor/MOpt-AFL).
|   |                           "run_afl_fuzz" function is modified to properly evaluate some benchmark programs.
...
```
If the users want to evaluate our fuzzers on the FuzzBench framework from original git, 
please modify the files above.
Otherwise, FuzzBench may not properly work on some benchmark programs we provide.

For more detail, please refer to [benchmarks](./benchmarks).

### Using old-version FuzzBench (the old-version - commit: f1c1291)

Users can set up all requirements and pull the dispatcher-image (the old version) by following the command below:
please follow the command below.
```
# install packages and docker
/workspace& sudo apt-get install -y python3-venv python3-dev build-essential libpq-dev python3-pip docker-compose
/workspace& sudo groupadd docker
/workspace& sudo gpasswd -a $USER docker
/workspace& newgrp docker

# git clone fuzzbench and use the old version(f1c1291)
/workspace$ git clone https://github.com/google/fuzzbench
/workspace$ git -C ./fuzzbench checkout f1c1291

# git clone our repositoy, apply patches and make venv setup with installing dispatcher docker image
/workspace& git clone https://github.com/kupl/SeamFuzz-Artifact
/workspace$ cp SeamFuzz-Aritfact/patches/* ./fuzzbench/
/workspace$ SeamFuzz-Aritfact/scripts/apply_patch.sh ./fuzzbench
/workspace$ SeamFuzz-Aritfact/scripts/setup_script.sh ./fuzzbench ./SeamFuzz-Artifact
```

### Example (Building artifact on Ubuntu 20.04.5)
The instructions below guide the users to build our artifact on [Ubuntu 20.04.5](https://releases.ubuntu.com/focal) wihtout any additional packages pre-installed. 



```
# No additional packages are pre-installed after installing Ubuntu 20.04.5_server. 
/workspace$ sudo apt-get install -y build-essential libpq-dev docker docker-compose libssl-dev openssl wget zlib1g-dev libffi-dev

/workspace$  sudo groupadd docker
/workspace$  sudo gpasswd -a $USER docker
/workspace$  newgrp docker

# Install python==3.9.15
/workspace$ wget https://www.python.org/ftp/python/3.9.15/Python-3.9.15.tgz
/workspace$ tar -xvf Python-3.9.15.tgz
/workspace$ pushd Python-3.9.15
/workspace$ ./configure --enable-optimizations
/workspace$ make -j$(nproc)
/workspace$ sudo make install
/workspace$ popd

# Update pip to 22.1.2
/workspace$ sudo ln -fs /usr/local/bin/python3.9 /usr/bin/python3
/workspace$ sudo ln -fs /usr/local/bin/python3.9 /usr/local/bin/python3
/workspace$ python3 -m pip install --upgrade pip==22.1.2
/workspace$ python3 -m pip install virtualenv docker-compose==1.25.0

/workspace$ git clone https://github.com/google/fuzzbench
/workspace$ git -C ./fuzzbench checkout f1c1291

/workspace$ git clone https://github.com/kupl/SeamFuzz-Artifact
/workspace$ cp ./SeamFuzz-Artifact/patches/* ./fuzzbench/
/workspace$ ./SeamFuzz-Artifact/scripts/apply_patches.sh ./fuzzbench
/workspace$ ./SeamFuzz-Artifact/scripts/setup_script.sh ./fuzzbench ./SeamFuzz-Artifact
```

The pip/python versions we used are as follows:


```
/workspace$ python3 -m pip –version
pip 22.1.2 from /home/seamfuzz/.local/lib/python3.9/site-packages/pip (python 3.9)

/workspace$ python3 –version
Python 3.9.15
```

Please make sure that we followed the commands above on the machine without any additional pre-installed packages after ubuntu installation. 


### Using VM(VirtualMachine) 
We also provide a VM image file which contain all contents to evaluate the experiments.
The VM image is built on [VirtualBox 7.0](https://www.virtualbox.org).
This VM is set to use 48GB memories and 8 CPU cores with 1.5 TB disk.
The sudo user password is set to "1234".
You can donwload the VM image from the following link.

[SeamFuzz VM](https://doi.org/10.5281/zenodo.7578055)

* Loading our VM image on VirtualBox with older version ( < 7.0) may raise an error.

## Building SeamFuzz without FuzzBench
The source code for SeamFuzz is on ./fuzzbench/fuzzers/seamfuzz/afl. 
As SeamFuzz is built on AFL++, the installation process for SeamFuzz is the same as AFL++.
Please follow the instructions in [AFL++](https://github.com/AFLplusplus/AFLplusplus) to build SeamFuzz on a local machine.
