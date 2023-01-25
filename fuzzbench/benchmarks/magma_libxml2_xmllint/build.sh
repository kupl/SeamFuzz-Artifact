#!/bin/bash -eu
#
# Copyright 2016 Google Inc.
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
#
################################################################################
cp $SRC/thompson.py $OUT/

cd $SRC/libxml2


./autogen.sh \
	--with-http=no \
	--with-python=no \
	--with-lzma=yes \
	--with-threads=no \
	--disable-shared
#./autogen.sh 
#./configure --without-python --with-threads=no --with-zlib=no --with-lzma=no

make -j$(nproc) clean
make -j$(nproc) all
make install

cp xmllint $OUT/

$CXX $CXXFLAGS -std=c++11 $SRC/xmllint_cov.cc $FUZZER_LIB -L /usr/lib -o $OUT/xmllint_cov

cp $SRC/xml.dict $OUT/libxml2_xml_read_memory_fuzzer.dict
zip -r $OUT/libxml2_xml_read_memory_fuzzer_seed_corpus.zip $SRC/libxml2/test
