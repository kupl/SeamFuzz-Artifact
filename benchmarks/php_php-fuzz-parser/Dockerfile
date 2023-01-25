# Copyright 2019 Google Inc.
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
RUN apt-get update && \
    apt-get install -y autoconf automake libtool bison re2c pkg-config

RUN git clone --no-checkout https://github.com/php/php-src.git php-src
RUN git -C $SRC/php-src checkout bc39abe8c3c492e29bc5d60ca58442040bbf063b

RUN git clone --no-checkout https://github.com/kkos/oniguruma.git php-src/oniguruma
RUN git -C $SRC/php-src/oniguruma checkout 227ec0bd690207812793c09ad70024707c405376

WORKDIR php-src
COPY build.sh *.options thompson.py $SRC/
ADD seeds $SRC/seeds
