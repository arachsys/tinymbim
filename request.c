#include <endian.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>

#include <arpa/inet.h>

#include "mbim.h"

static const uint8_t basic_connect[16] = {
  0xa2, 0x89, 0xcc, 0x33, 0xbc, 0xbb, 0x8b, 0x4f,
  0xb6, 0xb0, 0x13, 0x3e, 0xc2, 0xaa, 0xe6,0xdf
};

static const uint8_t context_internet[16] = {
  0x7e, 0x5e, 0x2a, 0x7e, 0x4e, 0x6f, 0x72, 0x72,
  0x73, 0x6b, 0x65, 0x6e, 0x7e, 0x5e, 0x2a, 0x7e
};

static void *begin(struct command_message *cmd, const uint8_t *uuid,
    uint32_t type, uint32_t cid, size_t length) {
  cmd->fragment_header.total = htole32(1);
  cmd->fragment_header.current = htole32(0);
  memcpy(cmd->service_id, uuid, 16);
  cmd->command_id = htole32(cid);
  cmd->command_type = htole32(type);
  cmd->buffer_length = htole32(length);
  return cmd->buffer;
}

static void finish(struct command_message *cmd) {
  cmd->header.type = htole32(MESSAGE_TYPE_COMMAND);
  cmd->header.length = sizeof(struct command_message)
    + le32toh(cmd->buffer_length);
}

static void string(struct command_message *cmd, struct reference *out,
    const char *in) {
  uint8_t *cursor = cmd->buffer + le32toh(cmd->buffer_length);
  uint8_t *limit = (uint8_t *) cmd + le32toh(cmd->header.length);
  ssize_t count, length = 0;
  mbstate_t state = { 0 };

  do {
    if (cursor + 2 > limit)
      errx(EXIT_FAILURE, "Request is too large");
    if ((count = mbrtoc16((uint16_t *) cursor, in, -1, &state)))
      cursor += 2, length += 2;
    if (count > 0)
      in += count;
  } while (count != 0);

  out->offset = cmd->buffer_length;
  out->length = htole32(length);

  cursor += 3 & -length; /* pad to 4-byte boundary */
  cmd->buffer_length = htole32(cursor - cmd->buffer);
}

static void request_open(int fd, int argc, char **argv) {
  struct open_message *open = buffer(fd);

  open->max_control_transfer = open->header.length;
  open->header.type = htole32(MESSAGE_TYPE_OPEN);
  open->header.length = htole32(sizeof(*open));
  transmit(fd, open);
  free(open);
}

static void request_close(int fd, int argc, char **argv) {
  struct close_message *close = buffer(fd);

  close->header.type = htole32(MESSAGE_TYPE_CLOSE);
  close->header.length = htole32(sizeof(*close));
  transmit(fd, close);
  free(close);
}

static void request_caps(int fd, int argc, char **argv) {
  struct command_message *cmd = buffer(fd);

  begin(cmd, basic_connect, COMMAND_TYPE_QUERY,
    BASIC_CONNECT_DEVICE_CAPS, 0);
  finish(cmd);
  transmit(fd, cmd);
  free(cmd);
}

static void request_subscriber(int fd, int argc, char **argv) {
  struct command_message *cmd = buffer(fd);

  begin(cmd, basic_connect, COMMAND_TYPE_QUERY,
    BASIC_CONNECT_SUBSCRIBER_READY_STATUS, 0);
  finish(cmd);
  transmit(fd, cmd);
  free(cmd);
}

static void request_radio(int fd, int argc, char **argv) {
  struct command_message *cmd = buffer(fd);

  if (argc > 0) {
    struct basic_connect_radio_state_s *state = begin(cmd, basic_connect,
      COMMAND_TYPE_SET, BASIC_CONNECT_RADIO_STATE, sizeof(*state));
    if (strcmp(argv[0], "off") == 0 || strcmp(argv[0], "0") == 0)
      state->radio_state = htole32(RADIO_SWITCH_STATE_OFF);
    else
      state->radio_state = htole32(RADIO_SWITCH_STATE_ON);
  } else {
    begin(cmd, basic_connect, COMMAND_TYPE_QUERY, BASIC_CONNECT_RADIO_STATE,
      sizeof(struct basic_connect_radio_state_r));
  }

  finish(cmd);
  transmit(fd, cmd);
  free(cmd);
}

static void request_pin(int fd, int argc, char **argv) {
  struct command_message *cmd = buffer(fd);

  if (argc > 0) {
    struct basic_connect_pin_s *pin = begin(cmd, basic_connect,
        COMMAND_TYPE_SET, BASIC_CONNECT_PIN, sizeof(*pin));
    pin->pin_type = htole32(PIN_TYPE_PIN1);
    pin->pin_operation = htole32(PIN_OPERATION_ENTER);
    string(cmd, &pin->pin, argv[0]);
  } else {
    begin(cmd, basic_connect, COMMAND_TYPE_QUERY,
      BASIC_CONNECT_PIN, 0);
  }

  finish(cmd);
  transmit(fd, cmd);
  free(cmd);
}

static void request_home(int fd, int argc, char **argv) {
  struct command_message *cmd = buffer(fd);

  begin(cmd, basic_connect, COMMAND_TYPE_QUERY,
    BASIC_CONNECT_HOME_PROVIDER, 0);
  finish(cmd);
  transmit(fd, cmd);
  free(cmd);
}

static void request_register(int fd, int argc, char **argv) {
  struct command_message *cmd = buffer(fd);

  if (argc > 0) {
    struct basic_connect_register_state_s *state = begin(cmd, basic_connect,
      COMMAND_TYPE_SET, BASIC_CONNECT_REGISTER_STATE, sizeof(*state));
    if (strcmp(argv[0], "auto") == 0) {
      state->register_action = htole32(REGISTER_ACTION_AUTOMATIC);
    } else {
      state->register_action = htole32(REGISTER_ACTION_MANUAL);
      string(cmd, &state->provider_id, argv[0]);
    }
  } else {
    begin(cmd, basic_connect, COMMAND_TYPE_QUERY,
      BASIC_CONNECT_REGISTER_STATE, 0);
  }

  finish(cmd);
  transmit(fd, cmd);
  free(cmd);
}

static void request_attach(int fd, int argc, char **argv) {
  struct command_message *cmd = buffer(fd);
  struct basic_connect_packet_service_s *service = begin(cmd, basic_connect,
    COMMAND_TYPE_SET, BASIC_CONNECT_PACKET_SERVICE, sizeof(*service));

  service->packet_service_action = htole32(PACKET_SERVICE_ACTION_ATTACH);
  finish(cmd);
  transmit(fd, cmd);
  free(cmd);
}

static void request_detach(int fd, int argc, char **argv) {
  struct command_message *cmd = buffer(fd);
  struct basic_connect_packet_service_s *service = begin(cmd, basic_connect,
    COMMAND_TYPE_SET, BASIC_CONNECT_PACKET_SERVICE, sizeof(*service));

  service->packet_service_action = htole32(PACKET_SERVICE_ACTION_DETACH);
  finish(cmd);
  transmit(fd, cmd);
  free(cmd);
}

static void request_connect(int fd, int argc, char **argv) {
  struct command_message *cmd = buffer(fd);
  struct basic_connect_connect_s *connect = begin(cmd, basic_connect,
    COMMAND_TYPE_SET, BASIC_CONNECT_CONNECT, sizeof(*connect));

  connect->activation_command = htole32(ACTIVATION_COMMAND_ACTIVATE);
  connect->ip_type = htole32(CONTEXT_IP_TYPE_DEFAULT);
  memcpy(connect->context_type, context_internet, 16);

  if (argc > 0) {
    char *apn = index(argv[0], ':');
    if (apn == NULL) {
      apn = argv[0];
    } else {
      *apn++ = 0;
      if (strcmp(argv[0], "ipv4") == 0)
        connect->ip_type = htole32(CONTEXT_IP_TYPE_IPV4);
      else if (strcmp(argv[0], "ipv6") == 0)
        connect->ip_type = htole32(CONTEXT_IP_TYPE_IPV6);
      else if (strcmp(argv[0], "ipv4v6") == 0)
        connect->ip_type = htole32(CONTEXT_IP_TYPE_IPV4V6);
    }
    string(cmd, &connect->access_string, apn);
  }

  if (argc > 3) {
    if (strcmp(argv[1], "pap") == 0)
      connect->auth_protocol = htole32(AUTH_PROTOCOL_PAP);
    else if (strcmp(argv[1], "chap") == 0)
      connect->auth_protocol = htole32(AUTH_PROTOCOL_CHAP);
    else if (strcmp(argv[1], "mschapv2") == 0)
      connect->auth_protocol = htole32(AUTH_PROTOCOL_MSCHAPV2);
    if (connect->auth_protocol) {
      string(cmd, &connect->username, argv[2]);
      string(cmd, &connect->password, argv[3]);
    }
  }

  finish(cmd);
  transmit(fd, cmd);
  free(cmd);
}

static void request_disconnect(int fd, int argc, char **argv) {
  struct command_message *cmd = buffer(fd);
  struct basic_connect_connect_s *connect = begin(cmd, basic_connect,
    COMMAND_TYPE_SET, BASIC_CONNECT_CONNECT, sizeof(*connect));

  connect->activation_command = htole32(ACTIVATION_COMMAND_DEACTIVATE);
  memcpy(connect->context_type, context_internet, 16);
  finish(cmd);
  transmit(fd, cmd);
  free(cmd);
}

static void request_config(int fd, int argc, char **argv) {
  struct command_message *cmd = buffer(fd);

  begin(cmd, basic_connect, COMMAND_TYPE_QUERY,
    BASIC_CONNECT_IP_CONFIGURATION,
    sizeof(struct basic_connect_ip_configuration_q));
  finish(cmd);
  transmit(fd, cmd);
  free(cmd);
}

struct request_handler request_handlers[] = {
  { "open", request_open },
  { "close", request_close },
  { "caps", request_caps },
  { "subscriber", request_subscriber },
  { "radio", request_radio },
  { "pin", request_pin },
  { "home", request_home },
  { "register", request_register },
  { "attach", request_attach },
  { "detach", request_detach },
  { "connect", request_connect },
  { "disconnect", request_disconnect },
  { "config", request_config },
  { NULL, NULL }
};
