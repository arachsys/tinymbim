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
      int fd = device(argv[1]);
      request(fd, argc - 3, argv + 3);
      return response(fd);
    }
  }

  fprintf(stderr, "\
Usage: %s DEVICE COMMAND\n\
Commands:\n\
  open              initialise modem\n\
  close             shut down modem\n\
  caps              query device capabilities\n\
  subscriber        query subscriber ready-state\n\
  radio [on|off]    query or control radio power\n\
  pin [PIN]         query PIN status or unlock with PIN\n\
  home              query home provider details\n\
  visible           scan for visible providers\n\
  register [auto|PROVIDER]\n\
                    query or set provider registration state\n\
  attach            attach to packet service\n\
  detach            detach from packet service\n\
  signal            check signal strength and error rate\n\
  connect [ipv4:|ipv6:|ipv4v6:]APN [AUTH USER PASS]\n\
                    activate IP data session\n\
  disconnect        deactivate IP data session\n\
  config            query IP configuration for data session\n\
", argv[0]);
  return 64;
}
