# Benchmarks

We offer the benchmark programs we used for our evaluations used in our paper. All benchmark programs contain all components to be properly worked on FuzzBench framework, includeing thompson.py for SeamFuzz.

For most benchmark programs, users only need to put them in `benchmarks` directory in their own FuzzBench framework, 
but the following programs needs to modify some parts of the source code of Fuzzbench as explained in [INSTALL.md](./INSTALL.md):
* objdump
* infotocap
* podofo

