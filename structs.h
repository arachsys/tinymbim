struct reference {
  uint32_t offset;
  uint32_t length;
} __attribute__((packed));

struct header {
  uint32_t type;
  uint32_t length;
  uint32_t transaction_id;
} __attribute__((packed));

struct open_message {
  struct header header;
  uint32_t max_control_transfer;
} __attribute__((packed));

struct open_done_message {
  struct header header;
  uint32_t status_code;
} __attribute__((packed));

struct close_message {
  struct header header;
} __attribute__((packed));

struct close_done_message {
  struct header header;
  uint32_t status_code;
} __attribute__((packed));

struct error_message {
  struct header header;
  uint32_t error_status_code;
} __attribute__((packed));

struct fragment_header {
  uint32_t total;
  uint32_t current;
} __attribute__((packed));

struct fragment_message {
  struct fragment_header fragment_header;
  uint8_t buffer[];
} __attribute__((packed));

struct command_message {
  struct header header;
  struct fragment_header fragment_header;
  uint8_t service_id[16];
  uint32_t command_id;
  uint32_t command_type;
  uint32_t buffer_length;
  uint8_t buffer[];
} __attribute__((packed));

struct command_done_message {
  struct header header;
  struct fragment_header fragment_header;
  uint8_t service_id[16];
  uint32_t command_id;
  uint32_t status_code;
  uint32_t buffer_length;
  uint8_t buffer[];
} __attribute__((packed));

struct indicate_status_message {
  struct header header;
  struct fragment_header fragment_header;
  uint8_t service_id[16];
  uint32_t command_id;
  uint32_t buffer_length;
  uint8_t buffer[];
} __attribute__((packed));

struct basic_connect_device_caps_r {
  uint32_t device_type;
  uint32_t cellular_class;
  uint32_t voice_class;
  uint32_t sim_class;
  uint32_t data_class;
  uint32_t sms_caps;
  uint32_t control_caps;
  uint32_t max_sessions;
  struct reference custom_data_class;
  struct reference device_id;
  struct reference firmware_info;
  struct reference hardware_info;
} __attribute__((packed));

struct basic_connect_subscriber_ready_status_r {
  uint32_t ready_state;
  struct reference subscriber_id;
  struct reference sim_icc_id;
  uint32_t ready_info;
  uint32_t telephone_numbers_count;
  struct reference telephone_numbers[];
} __attribute__((packed));

struct basic_connect_subscriber_ready_status_n {
  uint32_t ready_state;
  struct reference subscriber_id;
  struct reference sim_icc_id;
  uint32_t ready_info;
  uint32_t telephone_numbers_count;
  struct reference telephone_numbers[];
} __attribute__((packed));

struct basic_connect_radio_state_r {
  uint32_t hw_radio_state;
  uint32_t sw_radio_state;
} __attribute__((packed));

struct basic_connect_radio_state_s {
  uint32_t radio_state;
} __attribute__((packed));

struct basic_connect_radio_state_n {
  uint32_t hw_radio_state;
  uint32_t sw_radio_state;
} __attribute__((packed));

struct basic_connect_pin_r {
  uint32_t pin_type;
  uint32_t pin_state;
  uint32_t remaining_attempts;
} __attribute__((packed));

struct basic_connect_pin_s {
  uint32_t pin_type;
  uint32_t pin_operation;
  struct reference pin;
  struct reference new_pin;
} __attribute__((packed));

struct pin_desc {
  uint32_t pin_mode;
  uint32_t pin_format;
  uint32_t pin_length_min;
  uint32_t pin_length_max;
} __attribute__((packed));

struct basic_connect_pin_list_r {
  struct pin_desc pin_desc_pin_1;
  struct pin_desc pin_desc_pin_2;
  struct pin_desc pin_desc_device_sim_pin;
  struct pin_desc pin_desc_device_first_sim_pin;
  struct pin_desc pin_desc_network_pin;
  struct pin_desc pin_desc_network_subset_pin;
  struct pin_desc pin_desc_service_provider_pin;
  struct pin_desc pin_desc_corporate_pin;
  struct pin_desc pin_desc_subsidy_lock;
  struct pin_desc pin_desc_custom;
} __attribute__((packed));

struct provider {
  struct reference provider_id;
  uint32_t provider_state;
  struct reference provider_name;
  uint32_t cellular_class;
  uint32_t rssi;
  uint32_t error_rate;
} __attribute__((packed));

struct basic_connect_home_provider_r {
  struct provider provider;
} __attribute__((packed));

struct basic_connect_home_provider_s {
  struct provider provider;
} __attribute__((packed));

struct basic_connect_preferred_providers_r {
  uint32_t providers_count;
  struct reference providers[];
} __attribute__((packed));

struct basic_connect_preferred_providers_s {
  uint32_t providers_count;
  struct reference providers[];
} __attribute__((packed));

struct basic_connect_preferred_providers_n {
  uint32_t providers_count;
  struct reference providers[];
} __attribute__((packed));

struct basic_connect_visible_providers_q {
  uint32_t action;
} __attribute__((packed));

struct basic_connect_visible_providers_r {
  uint32_t providers_count;
  struct reference providers[];
} __attribute__((packed));

struct basic_connect_register_state_r {
  uint32_t nw_error;
  uint32_t register_state;
  uint32_t register_mode;
  uint32_t available_data_classes;
  uint32_t current_cellular_class;
  struct reference provider_id;
  struct reference provider_name;
  struct reference roaming_text;
  uint32_t registration_flag;
} __attribute__((packed));

struct basic_connect_register_state_s {
  struct reference provider_id;
  uint32_t register_action;
  uint32_t data_class;
} __attribute__((packed));

struct basic_connect_register_state_n {
  uint32_t nw_error;
  uint32_t register_state;
  uint32_t register_mode;
  uint32_t available_data_classes;
  uint32_t current_cellular_class;
  struct reference provider_id;
  struct reference provider_name;
  struct reference roaming_text;
  uint32_t registration_flag;
} __attribute__((packed));

struct basic_connect_packet_service_r {
  uint32_t nw_error;
  uint32_t packet_service_state;
  uint32_t highest_available_data_class;
  uint64_t uplink_speed;
  uint64_t downlink_speed;
} __attribute__((packed));

struct basic_connect_packet_service_s {
  uint32_t packet_service_action;
} __attribute__((packed));

struct basic_connect_packet_service_n {
  uint32_t nw_error;
  uint32_t packet_service_state;
  uint32_t highest_available_data_class;
  uint64_t uplink_speed;
  uint64_t downlink_speed;
} __attribute__((packed));

struct basic_connect_signal_state_r {
  uint32_t rssi;
  uint32_t error_rate;
  uint32_t signal_strength_interval;
  uint32_t rssi_threshold;
  uint32_t error_rate_threshold;
} __attribute__((packed));

struct basic_connect_signal_state_s {
  uint32_t signal_strength_interval;
  uint32_t rssi_threshold;
  uint32_t error_rate_threshold;
} __attribute__((packed));

struct basic_connect_signal_state_n {
  uint32_t rssi;
  uint32_t error_rate;
  uint32_t signal_strength_interval;
  uint32_t rssi_threshold;
  uint32_t error_rate_threshold;
} __attribute__((packed));

struct basic_connect_connect_q {
  uint32_t session_id;
  uint32_t activation_state;
  uint32_t voice_call_state;
  uint32_t ip_type;
  uint8_t context_type[16];
  uint32_t nw_error;
} __attribute__((packed));

struct basic_connect_connect_r {
  uint32_t session_id;
  uint32_t activation_state;
  uint32_t voice_call_state;
  uint32_t ip_type;
  uint8_t context_type[16];
  uint32_t nw_error;
} __attribute__((packed));

struct basic_connect_connect_s {
  uint32_t session_id;
  uint32_t activation_command;
  struct reference access_string;
  struct reference username;
  struct reference password;
  uint32_t compression;
  uint32_t auth_protocol;
  uint32_t ip_type;
  uint8_t context_type[16];
} __attribute__((packed));

struct basic_connect_connect_n {
  uint32_t session_id;
  uint32_t activation_state;
  uint32_t voice_call_state;
  uint32_t ip_type;
  uint8_t context_type[16];
  uint32_t nw_error;
} __attribute__((packed));

struct provisioned_context_element {
  uint32_t context_id;
  uint8_t context_type[16];
  struct reference access_string;
  struct reference username;
  struct reference password;
  uint32_t compression;
  uint32_t auth_protocol;
} __attribute__((packed));

struct basic_connect_provisioned_contexts_r {
  uint32_t provisioned_contexts_count;
  struct reference provisioned_contexts[];
} __attribute__((packed));

struct basic_connect_provisioned_contexts_s {
  uint32_t context_id;
  uint8_t context_type[16];
  struct reference access_string;
  struct reference username;
  struct reference password;
  uint32_t compression;
  uint32_t auth_protocol;
  struct reference provider_id;
} __attribute__((packed));

struct basic_connect_provisioned_contexts_n {
  uint32_t provisioned_contexts_count;
  struct reference provisioned_contexts[];
} __attribute__((packed));

struct basic_connect_service_activation_r {
  uint32_t nw_error;
  uint32_t buffer;
} __attribute__((packed));

struct basic_connect_service_activation_s {
  uint32_t buffer;
} __attribute__((packed));

struct basic_connect_ip_configuration_q {
  uint32_t session_id;
  uint32_t ipv4_configuration_available;
  uint32_t ipv6_configuration_available;
  uint32_t ipv4_address_count;
  uint32_t ipv4_address;
  uint32_t ipv6_address_count;
  uint32_t ipv6_address;
  uint32_t ipv4_gateway;
  uint32_t ipv6_gateway;
  uint32_t ipv4_dns_server_count;
  uint32_t ipv4_dns_server;
  uint32_t ipv6_dns_server_count;
  uint32_t ipv6_dns_server;
  uint32_t ipv4_mtu;
  uint32_t ipv6_mtu;
} __attribute__((packed));

struct basic_connect_ip_configuration_r {
  uint32_t session_id;
  uint32_t ipv4_configuration_available;
  uint32_t ipv6_configuration_available;
  uint32_t ipv4_address_count;
  uint32_t ipv4_address;
  uint32_t ipv6_address_count;
  uint32_t ipv6_address;
  uint32_t ipv4_gateway;
  uint32_t ipv6_gateway;
  uint32_t ipv4_dns_server_count;
  uint32_t ipv4_dns_server;
  uint32_t ipv6_dns_server_count;
  uint32_t ipv6_dns_server;
  uint32_t ipv4_mtu;
  uint32_t ipv6_mtu;
} __attribute__((packed));

struct basic_connect_ip_configuration_n {
  uint32_t session_id;
  uint32_t ipv4_configuration_available;
  uint32_t ipv6_configuration_available;
  uint32_t ipv4_address_count;
  uint32_t ipv4_address;
  uint32_t ipv6_address_count;
  uint32_t ipv6_address;
  uint32_t ipv4_gateway;
  uint32_t ipv6_gateway;
  uint32_t ipv4_dns_server_count;
  uint32_t ipv4_dns_server;
  uint32_t ipv6_dns_server_count;
  uint32_t ipv6_dns_server;
  uint32_t ipv4_mtu;
  uint32_t ipv6_mtu;
} __attribute__((packed));

struct device_service_element {
  uint8_t device_service_id[16];
  uint32_t dss_payload;
  uint32_t max_dss_instances;
  uint32_t cids_count;
  uint32_t cids[];
} __attribute__((packed));

struct basic_connect_device_services_r {
  uint32_t device_services_count;
  uint32_t max_dss_sessions;
  struct reference device_services[];
} __attribute__((packed));

struct event_entry {
  uint8_t device_service_id[16];
  uint32_t cids_count;
  uint32_t cids[];
} __attribute__((packed));

struct basic_connect_device_service_subscribe_list_r {
  uint32_t events_count;
  struct reference events[];
} __attribute__((packed));

struct basic_connect_device_service_subscribe_list_s {
  uint32_t events_count;
  struct reference events[];
} __attribute__((packed));

struct basic_connect_packet_statistics_r {
  uint32_t in_discards;
  uint32_t in_errors;
  uint64_t in_octets;
  uint64_t in_packets;
  uint64_t out_octets;
  uint64_t out_packets;
  uint32_t out_errors;
  uint32_t out_discards;
} __attribute__((packed));

struct basic_connect_network_idle_hint_r {
  uint32_t state;
} __attribute__((packed));

struct basic_connect_network_idle_hint_s {
  uint32_t state;
} __attribute__((packed));

struct basic_connect_emergency_mode_r {
  uint32_t state;
} __attribute__((packed));

struct basic_connect_emergency_mode_s {
  uint32_t state;
} __attribute__((packed));

struct basic_connect_emergency_mode_n {
  uint32_t state;
} __attribute__((packed));

struct packet_filter {
  uint32_t filter_size;
  uint32_t packet_filter;
  uint32_t packet_mask;
} __attribute__((packed));

struct basic_connect_ip_packet_filters_q {
  uint32_t session_id;
  uint32_t packet_filters_count;
  struct reference packet_filters[];
} __attribute__((packed));

struct basic_connect_ip_packet_filters_r {
  uint32_t session_id;
  uint32_t packet_filters_count;
  struct reference packet_filters[];
} __attribute__((packed));

struct basic_connect_ip_packet_filters_s {
  uint32_t session_id;
  uint32_t packet_filters_count;
  struct reference packet_filters[];
} __attribute__((packed));

struct basic_connect_multicarrier_providers_r {
  uint32_t providers_count;
  struct reference providers[];
} __attribute__((packed));

struct basic_connect_multicarrier_providers_s {
  uint32_t providers_count;
  struct reference providers[];
} __attribute__((packed));

struct basic_connect_multicarrier_providers_n {
  uint32_t providers_count;
  struct reference providers[];
} __attribute__((packed));
