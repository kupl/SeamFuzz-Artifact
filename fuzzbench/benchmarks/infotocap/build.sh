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

export FUZZ_TARGET=infotocap

cp -r seeds $OUT/
cp $SRC/thompson.py $OUT/
tar -xvf ncurses-6.2.tar.gz

#export LDFLAGS="-lpthread"

# Build fuzz target in $OUT directory.

cd $SRC/ncurses-6.2 && ./configure && make && make install && cp /usr/bin/infotocap $OUT/

cd $SRC/

$CXX $CXXFLAGS -std=c++11 infotocap_cov.cc $FUZZER_LIB -L /usr/local/lib -lz -o $OUT/infotocap_cov
