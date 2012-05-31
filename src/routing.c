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
        struct bus_hdr* hdr = get_bus_header(data + sizeof(uint16_t));

        dest_bus = NULL;
        for(i = 0; i < n_busses && dest_bus == NULL; i++) {
                for(node = busses[i].layout; node; node = node->next) {
                        if(node->addr == hdr->daddr) {
                                dest_bus = &busses[i];
                                break;
                        }
                }
        }

        if(dest_bus)
                bus_write(dest_bus, data, len);

}


#endif /* BUS_NODETYPE == BUS_ROOT */
