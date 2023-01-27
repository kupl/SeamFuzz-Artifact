# Benchmarks

We offer the benchmark programs we used for our evaluations used in our paper. All benchmark programs contain all components to be properly worked on FuzzBench framework, includeing thompson.py for SeamFuzz.

For most benchmark programs in this directory, users only need to put them in `benchmarks` directory in their own FuzzBench framework for evaulation by following the command:

```
/SeamFuzz-Artifact$ cp -r ./benchmarks [FUZZBENCH_PATH]/
```

However, to evaluate the following programs needs modification on some parts of the source code in Fuzzbench as explained in [INSTALL.md](../INSTALL.md):
* objdump
* infotocap
* podofo

