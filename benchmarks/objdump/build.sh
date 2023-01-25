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

cp -r seeds $OUT/

cp $SRC/thompson.py $OUT/


rm -rf binutils-2.37

tar -xvf binutils-2.37.tar.gz

#cp -r $SRC/binutils-2.37 $SRC/binutils

# Build project.

cd $SRC/binutils-2.37 && ./configure && make -j4 && cp $SRC/binutils-2.37/binutils/objdump /usr/local/bin/ && cp $SRC/binutils-2.37/binutils/objdump $OUT/

# Build fuzz target in $OUT directory.

cd $SRC/

$CXX $CXXFLAGS -std=c++11 objdump_cov.cc $FUZZER_LIB -L /usr/lib -o $OUT/objdump_cov

#cd $SRC/binutils && ./configure --enable-libfuzzer && make -j 4 && cp $SRC/binutils/binutils/objdump $OUT/objudmp_cov

