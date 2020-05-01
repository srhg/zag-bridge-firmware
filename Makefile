DEFINES += PROJECT_CONF_H=\"project-conf.h\"
CONTIKI_PROJECT = zag_bridge
PROJECT_SOURCEFILES += \
	commands.c \
	serialization.c \
	serial_protocol.c \
	zag_bridge_rdc.c
TARGET = cc2530dk

CONTIKI_WITH_RIME = 1

# For CC2530/CC2531, enable banking
HAVE_BANKING = 1

all: $(CONTIKI_PROJECT)

CONTIKI = contiki
include $(CONTIKI)/Makefile.include

.PHONY: flash
flash: zag_bridge.hex
	cc-tool -e -w zag_bridge.hex

