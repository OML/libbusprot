#ifndef _PACKETS_H
#define _PACKETS_H

#include "bus.h"

int bus_send_hello(struct bus_descriptor* bus, bus_addr_t new_addr);

#endif /* packets.h */
