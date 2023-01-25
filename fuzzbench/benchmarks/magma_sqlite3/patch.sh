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

patch -p1 < $SRC/patches/bugs/SQL001.patch
patch -p1 < $SRC/patches/bugs/SQL002.patch
patch -p1 < $SRC/patches/bugs/SQL003.patch
patch -p1 < $SRC/patches/bugs/SQL004.patch
patch -p1 < $SRC/patches/bugs/SQL005.patch
patch -p1 < $SRC/patches/bugs/SQL006.patch
patch -p1 < $SRC/patches/bugs/SQL007.patch
patch -p1 < $SRC/patches/bugs/SQL008.patch
patch -p1 < $SRC/patches/bugs/SQL009.patch
patch -p1 < $SRC/patches/bugs/SQL010.patch
patch -p1 < $SRC/patches/bugs/SQL011.patch
patch -p1 < $SRC/patches/bugs/SQL012.patch
patch -p1 < $SRC/patches/bugs/SQL013.patch
patch -p1 < $SRC/patches/bugs/SQL014.patch
patch -p1 < $SRC/patches/bugs/SQL015.patch
patch -p1 < $SRC/patches/bugs/SQL016.patch
patch -p1 < $SRC/patches/bugs/SQL017.patch
patch -p1 < $SRC/patches/bugs/SQL018.patch
patch -p1 < $SRC/patches/bugs/SQL019.patch
patch -p1 < $SRC/patches/bugs/SQL020.patch
patch -p1 < $SRC/patches/setup/tclexec-ignore-stderr.patch

