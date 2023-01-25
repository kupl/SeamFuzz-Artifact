// Copyright 2016 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <fcntl.h>
#include <libgen.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cassert>
#include <exception>
#include <iostream>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

/*
extern "C" int ignore_stdout(void) {
  int fd = open("/dev/null", O_WRONGLY);
  if (fd == -1) {
    warn("open(\"/dev/null\") failed");
    return -1;
  }

  int ret = 0;
  if (dup2(fd, STDOUT_FILENO) == -1) {
    warn("failed to redirect stdout to /dev/null\n");
    return -1;
  }

  if (close(fd) == -1) {
    warn("close");
    return -1;
  }

  return ret;
}
*/

extern "C" int delete_file(const char *pathname) {
  int ret = unlink(pathname);
  if (ret == -1) {
    warn("failed to delete \"%s\"", pathname);
  }

  free((void *)pathname);

  return ret;
}

extern "C" char *buf_to_file(const uint8_t *buf, size_t size) {
  char *pathname = strdup("/dev/shm/fuzz-XXXXXX");
  if (pathname == nullptr) {
    return nullptr;
  }

  int fd = mkstemp(pathname);
  if (fd == -1) {
    warn("mkstemp(\"%s\")", pathname);
    free(pathname);
    return nullptr;
  }

  size_t pos = 0;
  while (pos < size) {
    int nbytes = write(fd, &buf[pos], size - pos);
    if (nbytes <= 0) {
      if (nbytes == -1 && errno == EINTR) continue;
      warn("write");
      goto err;
    }

    pos += nbytes;
  }

  if (close(fd) == -1) {
    warn("close");
    goto err;
  }

  return pathname;

err:
  delete_file(pathname);
  return nullptr;
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {

  const char* file = buf_to_file(data, size);

  if (file == NULL) exit(EXIT_FAILURE);

  const std::string command = "./infotocap " + std::string(file);

  std::system ((const char *) command.c_str ());

  if (delete_file(file) != 0) {
    exit(EXIT_FAILURE);
  }

  return 0;
}
