#include "commands.h"
#include "net/netstack.h"
#include "serial_protocol.h"
#include "serialization.h"
#include "dev/leds.h"

static uint8_t buf_s[64];

static void commands_handle_radio_get_mem(const uint8_t *data, size_t data_len)
{
    uint8_t* reg;
    uint8_t len;
    int i;

    if (data_len < 3) {
        commands_send_err(NULL, 0);
        return;
    }

    reg = (uint8_t*)0 + deserialize_uint(&data[0], 2);
    len = (uint8_t)deserialize_uint(&data[2], 1);

    if (len > sizeof(buf_s)) {
        commands_send_err(NULL, 0);
        return;
    }

    for (i = 0; i < len; i++) {
        buf_s[i] = *reg++;
    }

    commands_send_ok(buf_s, len);
}

static void commands_handle_radio_set_mem(const uint8_t *data, size_t data_len)
{
    uint8_t* reg;
    int i;

    reg = (uint8_t*)0 + deserialize_uint(&data[0], 2);
    for (i = 0; i < data_len - 2; i++) {
        *(reg + i) = (uint8_t)deserialize_uint(&data[i], 1);
    }

    commands_send_ok(NULL, 0);
}

static void commands_handle_radio_get_mem_rev(const uint8_t *data,
                                              size_t data_len)
{
    uint8_t* reg;
    uint8_t len;
    int i;

    if (data_len < 3) {
        commands_send_err(NULL, 0);
        return;
    }

    reg = (uint8_t*)0 + deserialize_uint(&data[0], 2);
    len = (uint8_t)deserialize_uint(&data[2], 1);

    if (len > sizeof(buf_s)) {
        commands_send_err(NULL, 0);
        return;
    }

    for (i = 0; i < len; i++) {
        buf_s[(len - 1) - i] = *reg++;
    }

    commands_send_ok(buf_s, len);
}

static void commands_handle_radio_set_mem_rev(const uint8_t *data,
                                              size_t data_len)
{
    uint8_t* reg;
    int i;

    reg = (uint8_t*)0 + deserialize_uint(&data[0], 2);
    for (i = 0; i < data_len - 2; i++) {
        *(reg + i) = (uint8_t)deserialize_uint(&data[(data_len - 1) - i], 1);
    }

    commands_send_ok(NULL, 0);
}


static void commands_handle_radio_get_value(radio_param_t param)
{
    radio_value_t value = 0;
    radio_result_t result;
    uint8_t serialized_response[4];
    result = NETSTACK_RADIO.get_value(param, &value);
    serialize_int(result, &serialized_response[0], 2);
    serialize_int(value, &serialized_response[2], 2);
    commands_send_ok(serialized_response, sizeof(serialized_response));
}

static void commands_handle_radio_set_value(const uint8_t *data,
                                            size_t data_len)
{
    radio_param_t param;
    radio_value_t value;
    if (data_len < 4) {
        commands_send_err(NULL, 0);
        return;
    }
    param = deserialize_uint(&data[0], 2);
    value = deserialize_int(&data[2], 2);
    commands_send_ok_int((int)NETSTACK_RADIO.set_value(param, value));
}

static void commands_handle_radio_get_object(const uint8_t *data,
                                             size_t data_len)
{
  radio_param_t param;
  size_t expected_len;
  if (data_len < 3) {
    commands_send_err(NULL, 0);
    return;
  }
  param = deserialize_uint(&data[0], 2);
  expected_len = deserialize_uint(&data[2], 1);
  if (2 + expected_len > sizeof(buf_s)) {
    commands_send_err(NULL, 0);
    return;
  }
  serialize_int(
      (int)NETSTACK_RADIO.get_object(param, (void *)&buf_s[2], expected_len),
      &buf_s[0], 2);
  commands_send_ok(buf_s, 2 + expected_len);
}

static void commands_handle_radio_set_object(const uint8_t *data,
                                             size_t data_len)
{
    radio_param_t param;
    if (data_len < 2) {
        commands_send_err(NULL, 0);
        return;
    }
    param = deserialize_uint(data, data_len);
    commands_send_ok_int((int)NETSTACK_RADIO.set_object(param, &data[2],
                                                        data_len - 2));
}

static void commands_handle_io_get_leds(const uint8_t *data, size_t data_len)
{
    (void)data;
    (void)data_len;

    serialize_int(leds_get(), &buf_s[0], 1);
    commands_send_ok(buf_s, 1);
}

static void commands_handle_io_set_leds(const uint8_t *data, size_t data_len)
{
    uint8_t led_mask;
    uint8_t led_state;

    if (data_len < 2) {
        commands_send_err(NULL, 0);
        return;
    }

    led_mask = deserialize_uint(&data[0], 1);
    led_state = deserialize_uint(&data[1], 1);
    leds_on(led_mask & led_state);
    leds_off(led_mask & ~led_state);
    commands_send_ok(NULL, 0);
}

void commands_handle_command(command_t cmd, const uint8_t *data,
                             size_t data_len)
{
    switch (cmd) {
    case Command_Request_Radio_SendPacket:
        commands_send_ok_int(NETSTACK_RADIO.send(data, data_len));
        break;
    case Command_Request_Radio_GetMem:
        commands_handle_radio_get_mem(data, data_len);
        break;
    case Command_Request_Radio_SetMem:
        commands_handle_radio_set_mem(data, data_len);
        break;
    case Command_Request_Radio_GetMemRev:
        commands_handle_radio_get_mem_rev(data, data_len);
        break;
    case Command_Request_Radio_SetMemRev:
        commands_handle_radio_set_mem_rev(data, data_len);
        break;
    case Command_Request_Radio_GetValue:
        commands_handle_radio_get_value(
                (radio_param_t)deserialize_uint(data, data_len));
        break;
    case Command_Request_Radio_SetValue:
        commands_handle_radio_set_value(data, data_len);
        break;
    case Command_Request_Radio_GetObject:
        commands_handle_radio_get_object(data, data_len);
        break;
    case Command_Request_Radio_SetObject:
        commands_handle_radio_set_object(data, data_len);
        break;
    case Command_Request_IO_GetLeds:
        commands_handle_io_get_leds(data, data_len);
        break;
    case Command_Request_IO_SetLeds:
        commands_handle_io_set_leds(data, data_len);
        break;
    default:
        commands_send_err(NULL, 0);
        break;
    }
}

void commands_send_ok(const uint8_t *data, size_t data_len)
{
    serial_send_frame((uint8_t)Command_Response_OK, data, data_len);
}

void commands_send_err(const uint8_t *data, size_t data_len)
{
    serial_send_frame((uint8_t)Command_Response_Err, data, data_len);
}

void commands_send_ok_int(int data)
{
    uint8_t data_serialized[2];
    serialize_int(data, data_serialized, sizeof(data_serialized));
    commands_send_ok(data_serialized, sizeof(data_serialized));
}

void commands_send_err_int(int data)
{
    uint8_t data_serialized[2];
    serialize_int(data, data_serialized, sizeof(data_serialized));
    commands_send_err(data_serialized, sizeof(data_serialized));
}

void commands_send_event_on_packet(const uint8_t *data, size_t data_len,
                                   uint8_t rssi, uint8_t link_quality)
{
    uint8_t postfix[2] = {rssi, link_quality};
    serial_send_header((uint8_t)Command_Event_Radio_OnPacket,
                       data_len + sizeof(postfix));
    serial_send_data(data, data_len);
    serial_send_data(postfix, sizeof(postfix));
    serial_send_flush();
}

void commands_send_event_on_button(int button)
{
    uint8_t data_serialized[1];
    serialize_int(button, data_serialized, sizeof(data_serialized));
    serial_send_header((uint8_t)Command_Event_IO_OnButton,
                        sizeof(data_serialized));
    serial_send_data(data_serialized, sizeof(data_serialized));
    serial_send_flush();
}
