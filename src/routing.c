#include "bus.h"

#if BUS_NODETYPE == BUS_ROOT

#include "offsets.h"
#include "internal.h"
#include <stdlib.h>

void forward_packet(char* data, size_t len)
{
        int i;
        struct bus_node* node;
        struct bus_descriptor* dest_bus;
        struct bus_hdr* hdr = (struct bus_hdr*)data;
        struct bus_descriptor* bus;

        dest_bus = NULL;

        __builtin_nop();
        for(i = 0; i < n_busses && dest_bus == NULL; i++) {
                bus = &(busses[i]);
                for(node = bus->layout; node; node = node->next) {
                        __builtin_nop();
                        if(node->addr == hdr->daddr) {
                                dest_bus = &(busses[i]);
                                break;
                        }
                }
        }
        __builtin_nop();
        if(dest_bus)
                bus_write(dest_bus, data, len);

}


#endif /* BUS_NODETYPE == BUS_ROOT */
