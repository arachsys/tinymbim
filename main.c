#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mbim.h"

int main(int argc, char **argv) {
  void (*request)(int fd, int argc, char **argv) = NULL;
  int (*response)(int fd) = NULL;

  if (argc >= 3) {
    for (size_t i = 0; request_handlers[i].name; i++)
      if (strcmp(argv[2], request_handlers[i].name) == 0)
        request = request_handlers[i].handler;

    for (size_t i = 0; response_handlers[i].name; i++)
      if (strcmp(argv[2], response_handlers[i].name) == 0)
        response = response_handlers[i].handler;

    if (request && response) {
      int fd = open(argv[1], O_RDWR);
      if (fd < 0)
        err(EXIT_FAILURE, "open");
      request(fd, argc - 3, argv + 3);
      return response(fd);
    }
  }

  fprintf(stderr, "\
Usage: %s DEVICE COMMAND\n\
Commands:\n\
  open\n\
  close\n\
  caps\n\
  subscriber\n\
  radio [on|off]\n\
  pin [PIN]\n\
  home\n\
  register\n\
  attach\n\
  detach\n\
  signal\n\
  connect APN [AUTH USER PASS]\n\
  disconnect\n\
  config\n\
", argv[0]);
  return 64;
}
