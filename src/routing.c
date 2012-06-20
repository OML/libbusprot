#include "bus.h"

#if BUS_NODETYPE == BUS_ROOT

#include "offsets.h"
#include "internal.h"
#include <stdlib.h>

void forward_packet(char* data, size_t len)
{
        size_t i;
        struct bus_descriptor* dest_bus;
        struct bus_hdr* hdr = (struct bus_hdr*)data;

        if(hdr->len > 128)
                __builtin_nop();

        dest_bus = NULL;
        for(i = 0; i < n_nodes; i++) {
                if((nodes[i].devtype == hdr->dtype || (i == hdr->daddr && i > 0)) && nodes[i].bus != NULL) {
                        bus_write(nodes[i].bus, data, len);
                }
        }
}


#endif /* BUS_NODETYPE == BUS_ROOT */
