#!/bin/bash -eu
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

patch -p1 < $SRC/patches/bugs/SSL001.patch
patch -p1 < $SRC/patches/bugs/SSL002.patch
patch -p1 < $SRC/patches/bugs/SSL003.patch
patch -p1 < $SRC/patches/bugs/SSL004.patch
patch -p1 < $SRC/patches/bugs/SSL005.patch
patch -p1 < $SRC/patches/bugs/SSL006.patch
patch -p1 < $SRC/patches/bugs/SSL007.patch
patch -p1 < $SRC/patches/bugs/SSL008.patch
patch -p1 < $SRC/patches/bugs/SSL009.patch
patch -p1 < $SRC/patches/bugs/SSL010.patch
patch -p1 < $SRC/patches/bugs/SSL011.patch
patch -p1 < $SRC/patches/bugs/SSL012.patch
patch -p1 < $SRC/patches/bugs/SSL013.patch
patch -p1 < $SRC/patches/bugs/SSL014.patch
patch -p1 < $SRC/patches/bugs/SSL015.patch
patch -p1 < $SRC/patches/bugs/SSL016.patch
patch -p1 < $SRC/patches/bugs/SSL017.patch
patch -p1 < $SRC/patches/bugs/SSL018.patch
patch -p1 < $SRC/patches/bugs/SSL019.patch
patch -p1 < $SRC/patches/bugs/SSL020.patch
patch -p1 < $SRC/patches/assert.patch
