#include <stddef.h>

#include "mbim.h"

struct map device_type_values[] = {
  { DEVICE_TYPE_UNKNOWN, "unknown" },
  { DEVICE_TYPE_EMBEDDED, "embedded" },
  { DEVICE_TYPE_REMOVABLE, "removable" },
  { DEVICE_TYPE_REMOTE, "remote" },
  { 0, NULL }
};

struct map cellular_class_values[] = {
  { CELLULAR_CLASS_GSM, "gsm" },
  { CELLULAR_CLASS_CDMA, "cdma" },
  { 0, NULL }
};

struct map voice_class_values[] = {
  { VOICE_CLASS_UNKNOWN, "unknown" },
  { VOICE_CLASS_NO_VOICE, "none" },
  { VOICE_CLASS_SEPARATED_VOICE_DATA, "separated" },
  { VOICE_CLASS_SIMULTANEOUS_VOICE_DATA, "simultaneous" },
  { 0, NULL }
};

struct map sim_class_values[] = {
  { SIM_CLASS_LOGICAL, "logical" },
  { SIM_CLASS_REMOVABLE, "removable" },
  { 0, NULL }
};

struct map data_class_values[] = {
  { DATA_CLASS_GPRS, "gprs" },
  { DATA_CLASS_EDGE, "edge" },
  { DATA_CLASS_UMTS, "umts" },
  { DATA_CLASS_HSDPA, "hsdpa" },
  { DATA_CLASS_HSUPA, "hsupa" },
  { DATA_CLASS_LTE, "lte" },
  { DATA_CLASS_1XRTT, "1xrtt" },
  { DATA_CLASS_1XEVDO, "1xevdo" },
  { DATA_CLASS_1XEVDO_REVA, "1xevdo-reva" },
  { DATA_CLASS_1XEVDV, "1xevdv" },
  { DATA_CLASS_3XRTT, "3xrtt" },
  { DATA_CLASS_1XEVDO_REVB, "1xevdo-revb" },
  { DATA_CLASS_UMB, "umb" },
  { DATA_CLASS_CUSTOM, "custom" },
  { 0, NULL }
};

struct map sms_caps_values[] = {
  { SMS_CAPS_PDU_RECEIVE, "pdu-receive" },
  { SMS_CAPS_PDU_SEND, "pdu-send" },
  { SMS_CAPS_TEXT_RECEIVE, "text-receive" },
  { SMS_CAPS_TEXT_SEND, "text-send" },
  { 0, NULL }
};

struct map ctrl_caps_values[] = {
  { CTRL_CAPS_REG_MANUAL, "reg-manual" },
  { CTRL_CAPS_HW_RADIO_SWITCH, "hw-radio-switch" },
  { CTRL_CAPS_CDMA_MOBILE_IP, "cdma-mobile-ip" },
  { CTRL_CAPS_CDMA_SIMPLE_IP, "cdma-simple-ip" },
  { CTRL_CAPS_MULTI_CARRIER, "multi-carrier" },
  { 0, NULL }
};

struct map subscriber_ready_state_values[] = {
  { SUBSCRIBER_READY_STATE_NOT_INITIALIZED, "not-initialized" },
  { SUBSCRIBER_READY_STATE_INITIALIZED, "initialized" },
  { SUBSCRIBER_READY_STATE_SIM_NOT_INSERTED, "sim-not-inserted" },
  { SUBSCRIBER_READY_STATE_BAD_SIM, "bad-sim" },
  { SUBSCRIBER_READY_STATE_FAILURE, "failure" },
  { SUBSCRIBER_READY_STATE_NOT_ACTIVATED, "not-activated" },
  { SUBSCRIBER_READY_STATE_DEVICE_LOCKED, "device-locked" },
  { 0, NULL }
};

struct map radio_switch_state_values[] = {
  { RADIO_SWITCH_STATE_OFF, "off" },
  { RADIO_SWITCH_STATE_ON, "on" },
  { 0, NULL }
};

struct map pin_type_values[] = {
  { PIN_TYPE_UNKNOWN, "unknown" },
  { PIN_TYPE_CUSTOM, "custom" },
  { PIN_TYPE_PIN1, "pin1" },
  { PIN_TYPE_PIN2, "pin2" },
  { PIN_TYPE_DEVICE_SIM_PIN, "device-sim-pin" },
  { PIN_TYPE_DEVICE_FIRST_SIM_PIN, "device-first-sim-pin" },
  { PIN_TYPE_NETWORK_PIN, "network-pin" },
  { PIN_TYPE_NETWORK_SUBSET_PIN, "network-subset-pin" },
  { PIN_TYPE_SERVICE_PROVIDER_PIN, "service-provider-pin" },
  { PIN_TYPE_CORPORATE_PIN, "corporate-pin" },
  { PIN_TYPE_SUBSIDY_PIN, "subsidy-pin" },
  { PIN_TYPE_PUK1, "puk1" },
  { PIN_TYPE_PUK2, "puk2" },
  { PIN_TYPE_DEVICE_FIRST_SIM_PUK, "device-first-sim-puk" },
  { PIN_TYPE_NETWORK_PUK, "network-puk" },
  { PIN_TYPE_NETWORK_SUBSET_PUK, "network-subset-puk" },
  { PIN_TYPE_SERVICE_PROVIDER_PUK, "service-provider-puk" },
  { PIN_TYPE_CORPORATE_PUK, "corporate-puk" },
  { 0, NULL }
};

struct map pin_state_values[] = {
  { PIN_STATE_UNLOCKED, "unlocked" },
  { PIN_STATE_LOCKED, "locked" },
  { 0, NULL }
};

struct map pin_mode_values[] = {
  { PIN_MODE_NOT_SUPPORTED, "not-supported" },
  { PIN_MODE_ENABLED, "enabled" },
  { PIN_MODE_DISABLED, "disabled" },
  { 0, NULL }
};

struct map pin_format_values[] = {
  { PIN_FORMAT_UNKNOWN, "unknown" },
  { PIN_FORMAT_NUMERIC, "numeric" },
  { PIN_FORMAT_ALPHANUMERIC, "alphanumeric" },
  { 0, NULL }
};

struct map provider_state_values[] = {
  { PROVIDER_STATE_UNKNOWN, "unknown" },
  { PROVIDER_STATE_HOME, "home" },
  { PROVIDER_STATE_FORBIDDEN, "forbidden" },
  { PROVIDER_STATE_PREFERRED, "preferred" },
  { PROVIDER_STATE_VISIBLE, "visible" },
  { PROVIDER_STATE_REGISTERED, "registered" },
  { PROVIDER_STATE_PREFERRED_MULTICARRIER, "preferred-multicarrier" },
  { 0, NULL }
};

struct map nw_error_values[] = {
  { NW_ERROR_UNKNOWN, "unknown" },
  { NW_ERROR_IMSI_UNKNOWN_IN_HLR, "imsi-unknown-in-hlr" },
  { NW_ERROR_ILLEGAL_MS, "illegal-ms" },
  { NW_ERROR_IMSI_UNKNOWN_IN_VLR, "imsi-unknown-in-vlr" },
  { NW_ERROR_IMEI_NOT_ACCEPTED, "imei-not-accepted" },
  { NW_ERROR_ILLEGAL_ME, "illegal-me" },
  { NW_ERROR_GPRS_NOT_ALLOWED, "gprs-not-allowed" },
  { NW_ERROR_GPRS_AND_NON_GPRS_NOT_ALLOWED, "gprs-and-non-gprs-not-allowed" },
  { NW_ERROR_MS_IDENTITY_NOT_DERIVED_BY_NETWORK, "ms-identity-not-derived-by-network" },
  { NW_ERROR_IMPLICITLY_DETACHED, "implicitly-detached" },
  { NW_ERROR_PLMN_NOT_ALLOWED, "plmn-not-allowed" },
  { NW_ERROR_LOCATION_AREA_NOT_ALLOWED, "location-area-not-allowed" },
  { NW_ERROR_ROAMING_NOT_ALLOWED_IN_LOCATION_AREA, "roaming-not-allowed-in-location-area" },
  { NW_ERROR_GPRS_NOT_ALLOWED_IN_PLMN, "gprs-not-allowed-in-plmn" },
  { NW_ERROR_NO_CELLS_IN_LOCATION_AREA, "no-cells-in-location-area" },
  { NW_ERROR_MSC_TEMPORARILY_NOT_REACHABLE, "msc-temporarily-not-reachable" },
  { NW_ERROR_NETWORK_FAILURE, "network-failure" },
  { NW_ERROR_MAC_FAILURE, "mac-failure" },
  { NW_ERROR_SYNCH_FAILURE, "synch-failure" },
  { NW_ERROR_CONGESTION, "congestion" },
  { NW_ERROR_GSM_AUTHENTICATION_UNACCEPTABLE, "gsm-authentication-unacceptable" },
  { NW_ERROR_NOT_AUTHORIZED_FOR_CSG, "not-authorized-for-csg" },
  { NW_ERROR_MISSING_OR_UNKNOWN_APN, "missing-or-unknown-apn" },
  { NW_ERROR_SERVICE_OPTION_NOT_SUPPORTED, "service-option-not-supported" },
  { NW_ERROR_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED, "requested-service-option-not-subscribed" },
  { NW_ERROR_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER, "service-option-temporarily-out-of-order" },
  { NW_ERROR_NO_PDP_CONTEXT_ACTIVATED, "no-pdp-context-activated" },
  { NW_ERROR_SEMANTICALLY_INCORRECT_MESSAGE, "semantically-incorrect-message" },
  { NW_ERROR_INVALID_MANDATORY_INFORMATION, "invalid-mandatory-information" },
  { NW_ERROR_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED, "message-type-non-existent-or-not-implemented" },
  { NW_ERROR_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE, "message-type-not-compatible-with-protocol-state" },
  { NW_ERROR_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED, "information-element-non-existent-or-not-implemented" },
  { NW_ERROR_CONDITIONAL_IE_ERROR, "conditional-ie-error" },
  { NW_ERROR_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE, "message-not-compatible-with-protocol-state" },
  { NW_ERROR_PROTOCOL_ERROR_UNSPECIFIED, "protocol-error-unspecified" },
  { 0, NULL }
};

struct map register_state_values[] = {
  { REGISTER_STATE_UNKNOWN, "unknown" },
  { REGISTER_STATE_DEREGISTERED, "deregistered" },
  { REGISTER_STATE_SEARCHING, "searching" },
  { REGISTER_STATE_HOME, "home" },
  { REGISTER_STATE_ROAMING, "roaming" },
  { REGISTER_STATE_PARTNER, "partner" },
  { REGISTER_STATE_DENIED, "denied" },
  { 0, NULL }
};

struct map register_mode_values[] = {
  { REGISTER_MODE_UNKNOWN, "unknown" },
  { REGISTER_MODE_AUTOMATIC, "automatic" },
  { REGISTER_MODE_MANUAL, "manual" },
  { 0, NULL }
};

struct map registration_flag_values[] = {
  { REGISTRATION_FLAG_NONE, "none" },
  { REGISTRATION_FLAG_MANUAL_SELECTION_NOT_AVAILABLE, "manual-selection-not-available" },
  { REGISTRATION_FLAG_PACKET_SERVICE_AUTOMATIC_ATTACH, "packet-service-automatic-attach" },
  { 0, NULL }
};

struct map packet_service_state_values[] = {
  { PACKET_SERVICE_STATE_UNKNOWN, "unknown" },
  { PACKET_SERVICE_STATE_ATTACHING, "attaching" },
  { PACKET_SERVICE_STATE_ATTACHED, "attached" },
  { PACKET_SERVICE_STATE_DETACHING, "detaching" },
  { PACKET_SERVICE_STATE_DETACHED, "detached" },
  { 0, NULL }
};

struct map compression_values[] = {
  { COMPRESSION_NONE, "none" },
  { COMPRESSION_ENABLE, "enable" },
  { 0, NULL }
};

struct map auth_protocol_values[] = {
  { AUTH_PROTOCOL_NONE, "none" },
  { AUTH_PROTOCOL_PAP, "pap" },
  { AUTH_PROTOCOL_CHAP, "chap" },
  { AUTH_PROTOCOL_MSCHAPV2, "mschapv2" },
  { 0, NULL }
};

struct map context_ip_type_values[] = {
  { CONTEXT_IP_TYPE_DEFAULT, "default" },
  { CONTEXT_IP_TYPE_IPV4, "ipv4" },
  { CONTEXT_IP_TYPE_IPV6, "ipv6" },
  { CONTEXT_IP_TYPE_IPV4V6, "ipv4v6" },
  { CONTEXT_IP_TYPE_IPV4_AND_IPV6, "ipv4-and-ipv6" },
  { 0, NULL }
};

struct map activation_state_values[] = {
  { ACTIVATION_STATE_UNKNOWN, "unknown" },
  { ACTIVATION_STATE_ACTIVATED, "activated" },
  { ACTIVATION_STATE_ACTIVATING, "activating" },
  { ACTIVATION_STATE_DEACTIVATED, "deactivated" },
  { ACTIVATION_STATE_DEACTIVATING, "deactivating" },
  { 0, NULL }
};

struct map voice_call_state_values[] = {
  { VOICE_CALL_STATE_NONE, "none" },
  { VOICE_CALL_STATE_IN_PROGRESS, "in-progress" },
  { VOICE_CALL_STATE_HANG_UP, "hang-up" },
  { 0, NULL }
};

struct map ip_configuration_available_flag_values[] = {
  { IP_CONFIG_FLAG_NONE, "none" },
  { IP_CONFIG_FLAG_ADDRESS, "address" },
  { IP_CONFIG_FLAG_GATEWAY, "gateway" },
  { IP_CONFIG_FLAG_DNS, "dns" },
  { IP_CONFIG_FLAG_MTU, "mtu" },
  { 0, NULL }
};

struct map function_error_messages[] = {
  { ERROR_TIMEOUT_FRAGMENT, "Fragment was sent too late" },
  { ERROR_FRAGMENT_OUT_OF_SEQUENCE, "Fragment is out of sequence" },
  { ERROR_LENGTH_MISMATCH, "Command length does not match header" },
  { ERROR_DUPLICATED_TID, "Transaction ID was duplicated" },
  { ERROR_NOT_OPENED, "Device must be opened first" },
  { ERROR_UNKNOWN, "Unknown device error" },
  { ERROR_CANCEL, "Cancel pending transactions" },
  { ERROR_MAX_TRANSFER, "Invalid maximum transfer size" },
  { 0, NULL }
};

struct map command_error_messages[] = {
  { STATUS_BUSY, "Device is busy" },
  { STATUS_FAILURE, "Operation failed" },
  { STATUS_SIM_NOT_INSERTED, "SIM card is not inserted" },
  { STATUS_BAD_SIM, "SIM card cannot be used" },
  { STATUS_PIN_REQUIRED, "PIN is required" },
  { STATUS_PIN_DISABLED, "PIN is disabled" },
  { STATUS_NOT_REGISTERED, "Device is not registered" },
  { STATUS_PROVIDERS_NOT_FOUND, "No network providers found" },
  { STATUS_NO_DEVICE_SUPPORT, "Operation is not supported" },
  { STATUS_PROVIDER_NOT_VISIBLE, "Provider is not visible" },
  { STATUS_DATA_CLASS_NOT_AVAILABLE, "Data class is not available" },
  { STATUS_PACKET_SERVICE_DETACHED, "Packet service is detached" },
  { STATUS_MAX_ACTIVATED_CONTEXTS, "Cannot activate more contexts" },
  { STATUS_NOT_INITIALIZED, "Device is not initialized" },
  { STATUS_VOICE_CALL_IN_PROGRESS, "Voice call is in progress" },
  { STATUS_CONTEXT_NOT_ACTIVATED, "Context is not activated" },
  { STATUS_SERVICE_NOT_ACTIVATED, "Service is not activated" },
  { STATUS_INVALID_ACCESS_STRING, "Invalid access point name" },
  { STATUS_INVALID_USER_NAME_PWD, "Invalid username or password" },
  { STATUS_RADIO_POWER_OFF, "Radio is powered off" },
  { STATUS_INVALID_PARAMETERS, "Parameters are invalid" },
  { STATUS_READ_FAILURE, "Failed to read data" },
  { STATUS_WRITE_FAILURE, "Failed to write data" },
  { STATUS_OPERATION_NOT_ALLOWED, "Operation is not allowed" },
  { STATUS_CONTEXT_NOT_SUPPORTED, "Context is not supported" },
  { 0, NULL }
};
