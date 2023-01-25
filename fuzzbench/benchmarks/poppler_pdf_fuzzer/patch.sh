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

patch -p1 < $SRC/patches/bugs/PDF001.patch
patch -p1 < $SRC/patches/bugs/PDF002.patch
patch -p1 < $SRC/patches/bugs/PDF003.patch
patch -p1 < $SRC/patches/bugs/PDF004.patch
patch -p1 < $SRC/patches/bugs/PDF005.patch
patch -p1 < $SRC/patches/bugs/PDF006.patch
patch -p1 < $SRC/patches/bugs/PDF007.patch
patch -p1 < $SRC/patches/bugs/PDF008.patch
patch -p1 < $SRC/patches/bugs/PDF009.patch
patch -p1 < $SRC/patches/bugs/PDF010.patch
patch -p1 < $SRC/patches/bugs/PDF011.patch
patch -p1 < $SRC/patches/bugs/PDF012.patch
patch -p1 < $SRC/patches/bugs/PDF013.patch
patch -p1 < $SRC/patches/bugs/PDF014.patch
patch -p1 < $SRC/patches/bugs/PDF015.patch
patch -p1 < $SRC/patches/bugs/PDF016.patch
patch -p1 < $SRC/patches/bugs/PDF017.patch
patch -p1 < $SRC/patches/bugs/PDF018.patch
patch -p1 < $SRC/patches/bugs/PDF019.patch
patch -p1 < $SRC/patches/bugs/PDF020.patch
patch -p1 < $SRC/patches/bugs/PDF021.patch
patch -p1 < $SRC/patches/bugs/PDF022.patch

