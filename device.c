#include <dirent.h>
#include <endian.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <linux/usb/cdc-wdm.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>

#include "mbim.h"

void *buffer(int fd) {
  uint16_t size = 512;
  struct header *header;

  ioctl(fd, IOCTL_WDM_MAX_COMMAND, &size);
  header = calloc(size, 1);
  if (header == NULL)
    err(EXIT_FAILURE, "calloc");

  header->length = htole32(size);
  header->transaction_id = getpid();
  return header;
}

int device(const char *path) {
  int fd = open(path, O_RDWR);

  if (fd < 0)
    err(EXIT_FAILURE, "open");
  if (flock(fd, LOCK_EX) < 0)
    err(EXIT_FAILURE, "flock");
  return fd;
}

int interface(int fd) {
  struct stat status;
  char *path = NULL;
  size_t found = 0;

  if (fstat(fd, &status) < 0)
    err(EXIT_FAILURE, "fstat");
  if (!S_ISCHR(status.st_mode))
    return EXIT_FAILURE;

  if (asprintf(&path, "/sys/dev/char/%u:%u/device/net",
        major(status.st_rdev), minor(status.st_rdev)) >= 0) {
    struct dirent **list = NULL;
    ssize_t count = scandir(path, &list, NULL, alphasort);
    for (ssize_t i = 0; i < count; i++) {
      if (list[i]->d_name[0] != '.') {
        printf("interface %s\n", list[i]->d_name);
        found++;
      }
      free(list[i]);
    }
    free(list);
    free(path);
  }

  if (asprintf(&path, "/sys/dev/char/%u:%u/device/device/net",
        major(status.st_rdev), minor(status.st_rdev)) >= 0) {
    struct dirent **list = NULL;
    ssize_t count = scandir(path, &list, NULL, alphasort);
    for (ssize_t i = 0; i < count; i++) {
      if (list[i]->d_name[0] != '.') {
        printf("interface %s\n", list[i]->d_name);
        found++;
      }
      free(list[i]);
    }
    free(list);
    free(path);
  }

  if (found)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

void *receive(int fd, uint32_t type, uint32_t cid) {
  struct header *header = buffer(fd);
  ssize_t count, size = le32toh(header->length);
  uint32_t transaction = header->transaction_id;

  do {
    count = read(fd, header, size);
    if (count < 0) {
      if (errno != EAGAIN && errno != EINTR)
        err(EXIT_FAILURE, "read");
      continue;
    }
    if (count < (ssize_t) sizeof(struct header))
      errx(EXIT_FAILURE, "Response is truncated");
    if (count < le32toh(header->length))
      errx(EXIT_FAILURE, "Response is truncated");
  } while (header->transaction_id != transaction);

  if (le32toh(header->type) == MESSAGE_TYPE_FUNCTION_ERROR) {
    struct error_message *msg = (void *) header;
    if (count < (ssize_t) sizeof(struct error_message))
      errx(EXIT_FAILURE, "Error response is truncated");
    for (struct map *e = function_error_messages; e->value; e++)
      if (e->key == le32toh(msg->error_status_code))
        errx(EXIT_FAILURE, "%s", e->value);
    errx(EXIT_FAILURE, "Request failed with status %u",
      le32toh(msg->error_status_code));
  }

  if (le32toh(header->type) != type)
    errx(EXIT_FAILURE, "Response has unexpected type");

  if (le32toh(header->type) == MESSAGE_TYPE_COMMAND_DONE) {
    struct command_done_message *msg = (void *) header;
    count -= sizeof(struct command_done_message);
    if (count < 0 || count < le32toh(msg->buffer_length))
      errx(EXIT_FAILURE, "Response is truncated");
    if (msg->command_id != cid)
      errx(EXIT_FAILURE, "Response has unexpected command id");
    if (msg->status_code != STATUS_SUCCESS) {
      for (struct map *e = command_error_messages; e->value; e++)
        if (e->key == le32toh(msg->status_code))
          errx(EXIT_FAILURE, "%s", e->value);
      errx(EXIT_FAILURE, "Command failed with status %u",
        le32toh(msg->status_code));
    }
  }

  return header;
}

void transmit(int fd, const void *msg) {
  const struct header *header = msg;

  while (write(fd, header, le32toh(header->length)) < 0)
    if (errno != EAGAIN && errno != EINTR)
      err(EXIT_FAILURE, "write");
}
