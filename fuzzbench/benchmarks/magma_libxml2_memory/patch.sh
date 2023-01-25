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

patch -p1 < $SRC/patches/bugs/XML001.patch
patch -p1 < $SRC/patches/bugs/XML002.patch
patch -p1 < $SRC/patches/bugs/XML003.patch
patch -p1 < $SRC/patches/bugs/XML004.patch
patch -p1 < $SRC/patches/bugs/XML005.patch
patch -p1 < $SRC/patches/bugs/XML006.patch
patch -p1 < $SRC/patches/bugs/XML007.patch
patch -p1 < $SRC/patches/bugs/XML008.patch
patch -p1 < $SRC/patches/bugs/XML009.patch
patch -p1 < $SRC/patches/bugs/XML010.patch
patch -p1 < $SRC/patches/bugs/XML011.patch
patch -p1 < $SRC/patches/bugs/XML012.patch
patch -p1 < $SRC/patches/bugs/XML013.patch
patch -p1 < $SRC/patches/bugs/XML014.patch
patch -p1 < $SRC/patches/bugs/XML015.patch
patch -p1 < $SRC/patches/bugs/XML016.patch
patch -p1 < $SRC/patches/bugs/XML017.patch
patch -p1 < $SRC/patches/setup/libxml2.patch
patch -p1 < $SRC/patches/setup/stat.patch
