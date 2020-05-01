#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <stdint.h>
#include <string.h>

typedef enum {
  // Requests are sent USB -> Dongle
  Command_Request_Radio_SendPacket,
  Command_Request_Radio_GetMem,
  Command_Request_Radio_SetMem,
  Command_Request_Radio_GetMemRev,
  Command_Request_Radio_SetMemRev,
  Command_Request_Radio_GetValue,
  Command_Request_Radio_SetValue,
  Command_Request_Radio_GetObject,
  Command_Request_Radio_SetObject,
  Command_Request_IO_GetLeds,
  Command_Request_IO_SetLeds,

  // Responses are sent as replies to Requests, Dongle -> USB
  Command_Response_OK = 0x80,
  Command_Response_Err,

  // Sync is reserved
  Command_Sync_Reserved = 0xAA,
  
  // Events are sent Dongle -> USB
  Command_Event_Radio_OnPacket = 0xC0,
  Command_Event_IO_OnButton = 0xC1,
} command_t;

void commands_handle_command(command_t cmd, const uint8_t *data,
                             size_t data_len);
void commands_send_ok(const uint8_t *data, size_t data_len);
void commands_send_ok_int(int data);
void commands_send_err(const uint8_t *data, size_t data_len);
void commands_send_err_int(int data);
void commands_send_event_on_packet(const uint8_t *packet, size_t packet_len,
                                   uint8_t rssi, uint8_t link_quality);
void commands_send_event_on_button(int button);

#endif // _COMMANDS_H_
