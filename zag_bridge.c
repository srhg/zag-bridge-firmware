/*
 * Based on ZigbeeRadioBridge (https://github.com/Frans-Willem/ZigbeeRadioBridge)
 * 
 * Copyright (c) 2019, Frans-Willem Hardijzer <fw@hardijzer.nl>
 * Copyright (c) 2020, Jeff Kent <jeff@jkent.net>
 */

#include "commands.h"
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/io-arch.h"
#include "dev/leds.h"
#include "lib/ringbuf.h"
#include "net/netstack.h"
#include "serial_protocol.h"
#include "sys/process.h"
#include <stdbool.h>

PROCESS(packetbridge_process, "packet bridge process");
AUTOSTART_PROCESSES(&packetbridge_process);

static uint8_t receive_ringbuf_buffer[128];
static struct ringbuf receive_ringbuf;

static int on_serial_byte(uint8_t c) {
	ringbuf_put(&receive_ringbuf, c);
	process_poll(&packetbridge_process);
	return 1;
}

PROCESS_THREAD(packetbridge_process, ev, data) {
	PROCESS_BEGIN();
	(void)data; // Stop whining

	ringbuf_init(&receive_ringbuf, receive_ringbuf_buffer,
				 sizeof(receive_ringbuf_buffer));

	io_arch_set_input(on_serial_byte);

	while (true) {
		PROCESS_YIELD();
		if (ev == PROCESS_EVENT_POLL) {
			serial_process_input(&receive_ringbuf);
		} else if (ev == sensors_event) {
			if (data == &button_1_sensor) {
				commands_send_event_on_button(0);
			} else if (data == &button_2_sensor) {
				commands_send_event_on_button(1);
			}
		}
	}
	PROCESS_END();
}

void netstack_init(void) {
	NETSTACK_RADIO.init();
	NETSTACK_RDC.init();
}
