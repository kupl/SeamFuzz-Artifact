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

patch -p1 < $SRC/patches/bugs/PHP001.patch
patch -p1 < $SRC/patches/bugs/PHP002.patch
patch -p1 < $SRC/patches/bugs/PHP003.patch
patch -p1 < $SRC/patches/bugs/PHP004.patch
patch -p1 < $SRC/patches/bugs/PHP005.patch
patch -p1 < $SRC/patches/bugs/PHP006.patch
patch -p1 < $SRC/patches/bugs/PHP007.patch
patch -p1 < $SRC/patches/bugs/PHP008.patch
patch -p1 < $SRC/patches/bugs/PHP009.patch
patch -p1 < $SRC/patches/bugs/PHP010.patch
patch -p1 < $SRC/patches/bugs/PHP011.patch
patch -p1 < $SRC/patches/bugs/PHP012.patch
patch -p1 < $SRC/patches/bugs/PHP013.patch
patch -p1 < $SRC/patches/bugs/PHP014.patch
patch -p1 < $SRC/patches/bugs/PHP015.patch
patch -p1 < $SRC/patches/bugs/PHP016.patch
#patch -p1 < $SRC/patches/setup/setup.patch

