### Operating System 
We recommend using **Ubuntu 20.04** to run our artifact as the experiments were done on Ubuntu 20.04.
The artifact may not properly work on MacOS.

### Hardware Requirements
There is no strict restrictions on hardware, but we recommend to use machines with at least 20 cores and 128GB RAM
to run a single experiment(20 trials on a single benchmark program with a single fuzzer) properly.
The remaining hard disk space should be more than **600GB**.  

### Software Requirements
To properly evaluate FuzzBench framework provided by ours, we recommend to install **python-3.8**(recommended) or **python-3.9** and **pip >= 22.1.2 (22.1.2 is recommended)**. 
The `fuzzbench` directory in our repository is the commit version "f1c12914a4ca74073131c5744dfc6b33c33579be" of FuzzBench, 
which assumes the version of python is **python-3.9**. 
