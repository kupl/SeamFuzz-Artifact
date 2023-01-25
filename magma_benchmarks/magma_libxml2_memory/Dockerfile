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

FROM gcr.io/oss-fuzz-base/base-builder@sha256:1b6a6993690fa947df74ceabbf6a1f89a46d7e4277492addcd45a8525e34be5a

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y make autoconf automake libtool pkg-config python-dev python3-dev zlib1g-dev liblzma-dev

RUN git clone --no-checkout https://gitlab.gnome.org/GNOME/libxml2.git
RUN git -C $SRC/libxml2 checkout ec6e3efb06d7b15cf5a2328fabd3845acea4c815

ADD patches $SRC/patches
COPY patch.sh $SRC/libxml2/

RUN cd $SRC/libxml2/ && ./patch.sh

WORKDIR libxml2

COPY build.sh $SRC/
COPY thompson.py $SRC/
COPY *.cc *.h *.dict $SRC/
