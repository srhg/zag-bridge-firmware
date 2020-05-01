#include "serial_protocol.h"
#include "commands.h"
#include "dev/io-arch.h"
#include <stdbool.h>

#define LENGTH_SIZE 1

static struct {
    // Which state are we in ?
    enum {
        State_WaitingForCommand = 0,
        State_WaitingForLength,
        State_WaitingForData
    } state;
    /* In multi-byte states, this is the offset. e.g.
     * state=WaitingForLength, state_offset=1 means we're
     * waiting for the second byte of length data.
     */
    size_t offset;
    // Command identifier
    uint8_t command_id;
    // Length of data
    size_t data_length;
    uint8_t data[255];
} serial_state;

static void serial_reset_state()
{
    memset(&serial_state, 0, sizeof(serial_state));
}

void serial_process_input(struct ringbuf *input_buffer)
{
    static uint8_t sync = 0;

    int chr;
    while (true) {
        chr = ringbuf_get(input_buffer);
        if (chr == -1)
            break;
        switch (serial_state.state) {
        case State_WaitingForCommand: {
            if (serial_state.offset == 0) {
                if (chr == 0xAA) {
                        serial_state.offset++;
                } else {
                        serial_state.command_id = chr;
                        serial_state.state = State_WaitingForLength;
                }
            } else if (serial_state.offset == 1) {
                if (chr == 'Z') {
                        serial_state.offset++;
                } else {
                        serial_reset_state();
                }
            } else if (serial_state.offset == 2) {
                if (chr == 'A') {
                        serial_state.offset++;
                } else {
                        serial_reset_state();
                }
            } else if (serial_state.offset == 3) {
                if (chr == 'G') {
                        io_arch_writeb(0xAA);
                        io_arch_writeb('Z');
                        io_arch_writeb('A');
                        io_arch_writeb('G');
                }
                serial_reset_state();
            }
            break;
        }
        case State_WaitingForLength: {
            serial_state.data_length = chr;
            if (serial_state.data_length == 0) {
                commands_handle_command((command_t)serial_state.command_id,
                                        serial_state.data, serial_state.data_length);
                serial_reset_state();
            } else {
                serial_state.state = State_WaitingForData;
            }
            break;
        }
        case State_WaitingForData: {
            serial_state.data[serial_state.offset++] = chr;
            if (serial_state.offset >= serial_state.data_length) {
                commands_handle_command((command_t)serial_state.command_id,
                                        serial_state.data, serial_state.data_length);
                serial_reset_state();
            }
            break;
        }
        }
    }
}

void serial_send_header(uint8_t command, size_t data_len)
{
    size_t i;
    io_arch_writeb((uint8_t)command);
    io_arch_writeb((uint8_t)data_len);
}

void serial_send_data(const uint8_t *data, size_t data_len)
{
    size_t i;
    for (i = 0; i < data_len; i++) {
        io_arch_writeb(data[i]);
    }
}

void serial_send_flush()
{
    io_arch_flush();
}

void serial_send_frame(uint8_t command, const uint8_t *data,
                                             size_t data_len)
{
    serial_send_header(command, data_len);
    serial_send_data(data, data_len);
    serial_send_flush();
}
