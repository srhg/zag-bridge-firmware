#ifndef _SERIAL_PROTOCOL_H_
#define _SERIAL_PROTOCOL_H_

#include "lib/ringbuf.h"

void serial_process_input(struct ringbuf *input_buffer);
void serial_send_header(uint8_t command, size_t data_len);
void serial_send_data(const uint8_t* data, size_t data_len);
void serial_send_flush();
void serial_send_frame(uint8_t command, const uint8_t *data, size_t data_len);

#endif // _SERIAL_PROTOCOL_H_
