#include "commands.h"
#include "net/mac/mac.h"
#include "net/netstack.h"
#include "net/packetbuf.h"

static void zag_bridge_rdc_send(mac_callback_t sent, void *ptr)
{
    (void)sent;
    (void)ptr;
}

static void zag_bridge_rdc_send_list(mac_callback_t sent, void *ptr,
                                     struct rdc_buf_list *list)
{
    (void)sent;
    (void)ptr;
    (void)list;
}

static void zag_bridge_rdc_input(void)
{
    commands_send_event_on_packet((const uint8_t *)packetbuf_dataptr(),
            packetbuf_datalen(), packetbuf_attr(PACKETBUF_ATTR_RSSI),
            packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY));
}

static int zag_bridge_rdc_on(void)
{
    return 1;
}

static int zag_bridge_rdc_off(int keep_radio_on)
{
    return 0;
}

static unsigned short zag_bridge_rdc_cca(void)
{
    return 0;
}

static void zag_bridge_rdc_init(void)
{
    return;
}

const struct rdc_driver zag_bridge_rdc_driver = {
    "zag-bridge-rdc",
    zag_bridge_rdc_init,
    zag_bridge_rdc_send,
    zag_bridge_rdc_send_list,
    zag_bridge_rdc_input,
    zag_bridge_rdc_on,
    zag_bridge_rdc_off,
    zag_bridge_rdc_cca,
};
