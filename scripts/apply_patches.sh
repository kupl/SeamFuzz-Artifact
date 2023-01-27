#!/bin/bash -ex

pushd ${1}
patch -p1 < ./Makefile_patch.patch
patch -p1 < ./fuzzer_afl_patch.patch
patch -p1 < ./measurer_patch.patch
popd


