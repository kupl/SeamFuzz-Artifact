#!/bin/bash -ex
# Copyright 2020 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
cp $SRC/thompson.py $OUT/
cp -r $SRC/seeds $OUT/

mkdir $SRC/podofo-0.9.7/build

# Build project.

cd $SRC/podofo-0.9.7/build && cmake .. && make -j4 && cp $SRC/podofo-0.9.7/build/tools/podofopdfinfo/podofopdfinfo $OUT/
cp /usr/lib/x86_64-linux-gnu/*so* $OUT/
#cp /usr/lib/x86_64-linux-gnu/libfreetype* $OUT/

# Build fuzz target in $OUT directory.

cd $SRC/

$CXX $CXXFLAGS -std=c++11 podofo_cov.cc $FUZZER_LIB -L /usr/local/lib -o $OUT/podofo_cov

#cd $SRC/binutils && ./configure --enable-libfuzzer && make -j 4 && cp $SRC/binutils/binutils/objdump $OUT/objudmp_cov

