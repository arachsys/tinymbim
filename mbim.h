#ifndef MBIM_H
#define MBIM_H

#include <stdint.h>

#include "constants.h"
#include "strings.h"
#include "structs.h"

extern struct request_handler {
  const char *name;
  void (*handler)(int fd, int argc, char **argv);
} request_handlers[];

extern struct response_handler {
  const char *name;
  int (*handler)(int fd);
} response_handlers[];

void *buffer(int fd);
void interface(int fd);
void *receive(int fd, uint32_t type, uint32_t cid);
void transmit(int fd, const void *msg);

#endif
