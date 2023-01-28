# Fuzzers
We offer the fuzzers we used for our evaluations used in our paper. All fuzzers contain all components to be properly worked on FuzzBench framework.

* aflpp         : AFL++-3.15a
* aflppmopt     : Running MOpt option of AFL++-3.15a 
* seamfuzz      : Our tool based on AFL++-3.15a
* nocluster     : Only maintain one seed group (Table 3)
* eachcluster   : each seed input has their unique seed group (Table 3)
* naiveseamfuzz : do not utilize the data classification of ours, but only follows AFL++ criteria (Table 4)
* seamfuzz[N]   : The rewards are updated with the values of N  (Table 5)
