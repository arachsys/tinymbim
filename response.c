#include <endian.h>
#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <uchar.h>

#include <arpa/inet.h>

#include "mbim.h"

static void enum32(const char *key, uint32_t value, struct map *values) {
  if (values == NULL) {
    printf("%s %u\n", key, le32toh(value));
    return;
  }
  for (fputs(key, stdout); values->value; values++)
    if (values->key == le32toh(value))
      printf(" %s", values->value);
  putc('\n', stdout);
}

static void mask32(const char *key, uint32_t value, struct map *values) {
  if (values == NULL) {
    printf("%s %u\n", key, le32toh(value));
    return;
  }
  for (fputs(key, stdout); values->value; values++)
    if (values->key & le32toh(value))
      printf(" %s", values->value);
  putc('\n', stdout);
}

static void string(const char *key, const void *buffer, size_t size,
    const struct reference *value) {
  uint16_t *in = (uint16_t *) ((char *) buffer + le32toh(value->offset));
  ssize_t count, length = le32toh(value->length) >> 1;
  char out[MB_LEN_MAX];

  if (size < le32toh(value->offset) + le32toh(value->length))
    err(EXIT_FAILURE, "Response is truncated");
  if (le32toh(value->offset) == 0)
    return;

  printf("%s ", key);
  for (mbstate_t state = { 0 }; length > 0; in++, length--)
    if ((count = c16rtomb(out, le16toh(*in), &state)) > 0)
      for (ssize_t i = 0; i < count; i++)
        putc(out[i] & 224 && out[i] != 127 ? out[i] : 32, stdout);
  putc('\n', stdout);
}

static void ipv4(const char *key, const void *buffer, size_t size,
    uint32_t offset, size_t index, int mask) {
  char *in = (char *) buffer + le32toh(offset), out[INET_ADDRSTRLEN];

  if (size < le32toh(offset) + (index + 1) * (mask ? 8 : 4))
    err(EXIT_FAILURE, "Response is truncated");

  if (inet_ntop(AF_INET, in + (mask ? 4 + 8 * index : 4 * index),
        out, sizeof(out)) == NULL)
    printf("%s invalid\n", key);
  else if (mask)
    printf("%s %s/%u\n", key, out, le32toh(((uint32_t *) in)[2 * index]));
  else
    printf("%s %s\n", key, out);
}

static void ipv6(const char *key, const void *buffer, size_t size,
    uint32_t offset, size_t index, int mask) {
  char *in = (char *) buffer + le32toh(offset), out[INET6_ADDRSTRLEN];

  if (size < le32toh(offset) + (index + 1) * (mask ? 20 : 16))
    err(EXIT_FAILURE, "Response is truncated");

  if (inet_ntop(AF_INET6, in + (mask ? 4 + 20 * index : 16 * index),
        out, sizeof(out)) == NULL)
    printf("%s invalid\n", key);
  else if (mask)
    printf("%s %s/%u\n", key, out, le32toh(((uint32_t *) in)[5 * index]));
  else
    printf("%s %s\n", key, out);
}

static int response_open(int fd) {
  struct open_done_message *msg = receive(fd, MESSAGE_TYPE_OPEN_DONE, -1);

  if (msg->status_code == STATUS_SUCCESS)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

static int response_close(int fd) {
  struct close_done_message *msg = receive(fd, MESSAGE_TYPE_CLOSE_DONE, -1);

  if (msg->status_code == STATUS_SUCCESS)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

static int response_caps(int fd) {
  struct command_done_message *msg = receive(fd, MESSAGE_TYPE_COMMAND_DONE,
    BASIC_CONNECT_DEVICE_CAPS);
  struct basic_connect_device_caps_r *reply = (void *) msg->buffer;
  size_t size = le32toh(msg->buffer_length);

  if (size < sizeof(*reply))
    err(EXIT_FAILURE, "Response is truncated");

  enum32("device-type", reply->device_type, device_type_values);
  mask32("cellular-class", reply->cellular_class, cellular_class_values);
  enum32("voice-class", reply->voice_class, voice_class_values);
  mask32("sim-class", reply->sim_class, sim_class_values);
  mask32("data-class", reply->data_class, data_class_values);
  mask32("sms-caps", reply->sms_caps, sms_caps_values);
  mask32("control-caps", reply->control_caps, ctrl_caps_values);
  enum32("max-sessions", reply->max_sessions, NULL);
  string("device-id", reply, size, &reply->device_id);
  string("firmware-info", reply, size, &reply->firmware_info);
  string("hardware-info", reply, size, &reply->hardware_info);

  free(msg);
  return EXIT_SUCCESS;
}

static int response_subscriber(int fd) {
  struct command_done_message *msg = receive(fd, MESSAGE_TYPE_COMMAND_DONE,
    BASIC_CONNECT_SUBSCRIBER_READY_STATUS);
  struct basic_connect_subscriber_ready_status_r *reply
    = (void *) msg->buffer;
  size_t size = le32toh(msg->buffer_length);
  uint32_t state;

  if (size < sizeof(*reply))
    err(EXIT_FAILURE, "Response is truncated");
  state = le32toh(reply->ready_state);

  enum32("ready-state", reply->ready_state,
    subscriber_ready_state_values);
  string("sim-icc-id", reply, size, &reply->sim_icc_id);
  string("subscriber-id", reply, size, &reply->subscriber_id);
  if (le32toh(reply->ready_info) & READY_INFO_FLAG_PROTECT_UNIQUE_ID)
    printf("protect-unique-id 1\n");
  for (size_t i = 0; i < le32toh(reply->telephone_numbers_count); i++)
    string("telephone-number", reply, size, reply->telephone_numbers + i);
  free(msg);

  if (state == SUBSCRIBER_READY_STATE_INITIALIZED)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

static int response_radio(int fd) {
  struct command_done_message *msg = receive(fd, MESSAGE_TYPE_COMMAND_DONE,
    BASIC_CONNECT_RADIO_STATE);
  struct basic_connect_radio_state_r *reply = (void *) msg->buffer;

  if (le32toh(msg->buffer_length) < sizeof(*reply))
    err(EXIT_FAILURE, "Response is truncated");

  enum32("hw-radio", reply->hw_radio_state, radio_switch_state_values);
  enum32("sw-radio", reply->sw_radio_state, radio_switch_state_values);

  free(msg);
  return EXIT_SUCCESS;
}

static int response_pin(int fd) {
  struct command_done_message *msg = receive(fd, MESSAGE_TYPE_COMMAND_DONE,
    BASIC_CONNECT_PIN);
  struct basic_connect_pin_r *reply = (void *) msg->buffer;
  uint32_t state;

  if (le32toh(msg->buffer_length) < sizeof(*reply))
    err(EXIT_FAILURE, "Response is truncated");
  state = le32toh(reply->pin_state);

  enum32("pin-state", reply->pin_state, pin_state_values);
  if (le32toh(reply->pin_state) != PIN_STATE_UNLOCKED) {
    enum32("pin-type", reply->pin_type, pin_type_values);
    enum32("remaining-attempts", reply->remaining_attempts, NULL);
  }
  free(msg);

  if (state == PIN_STATE_UNLOCKED)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

static int response_home(int fd) {
  struct command_done_message *msg = receive(fd, MESSAGE_TYPE_COMMAND_DONE,
    BASIC_CONNECT_HOME_PROVIDER);
  struct basic_connect_home_provider_r *reply = (void *) msg->buffer;
  size_t size = le32toh(msg->buffer_length);

  if (size < sizeof(*reply))
    err(EXIT_FAILURE, "Response is truncated");

  string("provider-id", reply, size, &reply->provider.provider_id);
  string("provider-name", reply, size, &reply->provider.provider_name);
  mask32("provider-state", reply->provider.provider_state,
    provider_state_values);
  mask32("cellular-class", reply->provider.cellular_class,
    cellular_class_values);
  enum32("rssi", reply->provider.rssi, NULL);
  enum32("error-rate", reply->provider.error_rate, NULL);

  free(msg);
  return EXIT_SUCCESS;
}

static int response_register(int fd) {
  struct command_done_message *msg = receive(fd, MESSAGE_TYPE_COMMAND_DONE,
    BASIC_CONNECT_REGISTER_STATE);
  struct basic_connect_register_state_r *reply = (void *) msg->buffer;
  size_t size = le32toh(msg->buffer_length);
  uint32_t state;

  if (size < sizeof(*reply))
    err(EXIT_FAILURE, "Response is truncated");
  state = le32toh(reply->register_state);

  if (reply->nw_error)
    enum32("network-error", reply->nw_error, nw_error_values);
  enum32("register-state", reply->register_state,
    register_state_values);
  enum32("register-mode", reply->register_mode,
    register_mode_values);
  mask32("available-data-classes", reply->available_data_classes,
    data_class_values);
  mask32("current-cellular-class", reply->current_cellular_class,
    cellular_class_values);
  string("provider-id", reply, size, &reply->provider_id);
  string("provider-name", reply, size, &reply->provider_name);
  string("roaming-text", reply, size, &reply->roaming_text);
  free(msg);

  switch (state) {
    case REGISTER_STATE_HOME:
    case REGISTER_STATE_ROAMING:
    case REGISTER_STATE_PARTNER:
      return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

static uint32_t handle_attach(int fd) {
  struct command_done_message *msg = receive(fd, MESSAGE_TYPE_COMMAND_DONE,
    BASIC_CONNECT_PACKET_SERVICE);
  struct basic_connect_packet_service_r *reply = (void *) msg->buffer;
  uint32_t state;

  if (le32toh(msg->buffer_length) < sizeof(*reply))
    err(EXIT_FAILURE, "Response is truncated");
  state = le32toh(reply->packet_service_state);

  if (reply->nw_error)
    enum32("network-error", reply->nw_error, nw_error_values);
  enum32("packet-service-state", reply->packet_service_state,
    packet_service_state_values);
  printf("uplink-speed %ju\n", (intmax_t) le64toh(reply->uplink_speed));
  printf("downlink-speed %ju\n", (intmax_t) le64toh(reply->downlink_speed));

  free(msg);
  return state;
}

static int response_attach(int fd) {
  if (handle_attach(fd) == PACKET_SERVICE_STATE_ATTACHED)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

static int response_detach(int fd) {
  if (handle_attach(fd) == PACKET_SERVICE_STATE_DETACHED)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

static uint32_t handle_connect(int fd) {
  struct command_done_message *msg = receive(fd, MESSAGE_TYPE_COMMAND_DONE,
    BASIC_CONNECT_CONNECT);
  struct basic_connect_connect_r *reply = (void *) msg->buffer;
  uint32_t state;

  if (le32toh(msg->buffer_length) < sizeof(*reply))
    err(EXIT_FAILURE, "Response is truncated");
  state = le32toh(reply->activation_state);

  if (reply->nw_error)
    enum32("network-error", reply->nw_error, nw_error_values);
  enum32("session-id", reply->session_id, NULL);
  enum32("activation-state", reply->activation_state,
    activation_state_values);
  if (reply->voice_call_state)
    enum32("voice-call-state", reply->voice_call_state,
      voice_call_state_values);
  enum32("ip-type", reply->ip_type, context_ip_type_values);

  free(msg);
  return state;
}

static int response_connect(int fd) {
  if (handle_connect(fd) == ACTIVATION_STATE_ACTIVATED)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

static int response_disconnect(int fd) {
  if (handle_connect(fd) == ACTIVATION_STATE_DEACTIVATED)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

static int response_config(int fd) {
  struct command_done_message *msg = receive(fd, MESSAGE_TYPE_COMMAND_DONE,
    BASIC_CONNECT_IP_CONFIGURATION);
  struct basic_connect_ip_configuration_r *reply = (void *) msg->buffer;
  size_t size = le32toh(msg->buffer_length);
  uint32_t mtu = -1;

  if (size < sizeof(*reply))
    err(EXIT_FAILURE, "Response is truncated");
  interface(fd);

  if (le32toh(reply->ipv4_configuration_available) & IP_CONFIG_FLAG_ADDRESS)
    for (size_t i = 0; i < le32toh(reply->ipv4_address_count); i++)
      ipv4("address", reply, size, reply->ipv4_address, i, 1);
  if (le32toh(reply->ipv6_configuration_available) & IP_CONFIG_FLAG_ADDRESS)
    for (size_t i = 0; i < le32toh(reply->ipv6_address_count); i++)
      ipv6("address", reply, size, reply->ipv6_address, i, 1);

  if (le32toh(reply->ipv4_configuration_available) & IP_CONFIG_FLAG_DNS)
    ipv4("gateway", reply, size, reply->ipv4_gateway, 0, 0);
  if (le32toh(reply->ipv6_configuration_available) & IP_CONFIG_FLAG_DNS)
    ipv6("gateway", reply, size, reply->ipv6_gateway, 0, 0);

  if (le32toh(reply->ipv4_configuration_available) & IP_CONFIG_FLAG_DNS)
    for (size_t i = 0; i < le32toh(reply->ipv4_dns_server_count); i++)
      ipv4("dns", reply, size, reply->ipv4_dns_server, i, 0);
  if (le32toh(reply->ipv6_configuration_available) & IP_CONFIG_FLAG_DNS)
    for (size_t i = 0; i < le32toh(reply->ipv6_dns_server_count); i++)
      ipv6("dns", reply, size, reply->ipv6_dns_server, i, 0);

  if (le32toh(reply->ipv4_configuration_available) & IP_CONFIG_FLAG_MTU)
    if (le32toh(reply->ipv4_mtu) < mtu)
      mtu = le32toh(reply->ipv4_mtu);
  if (le32toh(reply->ipv6_configuration_available) & IP_CONFIG_FLAG_MTU)
    if (le32toh(reply->ipv6_mtu) < mtu)
      mtu = le32toh(reply->ipv6_mtu);
  if (mtu + 1 != 0)
    printf("mtu %u\n", mtu);

  free(msg);
  return EXIT_SUCCESS;
}

struct response_handler response_handlers[] = {
  { "open", response_open },
  { "close", response_close },
  { "caps", response_caps },
  { "subscriber", response_subscriber },
  { "radio", response_radio },
  { "pin", response_pin },
  { "home", response_home },
  { "register", response_register },
  { "attach", response_attach },
  { "detach", response_detach },
  { "connect", response_connect },
  { "disconnect", response_disconnect },
  { "config", response_config },
  { NULL, NULL }
};
