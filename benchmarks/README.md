# Benchmarks

We offer the benchmark programs we used for our evaluations used in our paper. All benchmark programs contain build.sh, Dockerfile, 
seeds(if existed), and thompson.py(for SeamFuzz) to be properly used on FuzzBench framework.

For most benchmark programs, users only need to put them in `benchmarks` directory in their own FuzzBench framework, 
but the following programs needs to modify some parts of the source code of Fuzzbench:
* objdump
* infotocap
* podofo

