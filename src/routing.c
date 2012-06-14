#include "bus.h"

#if BUS_NODETYPE == BUS_ROOT

#include "offsets.h"
#include "internal.h"
#include <stdlib.h>
int n_dt_3 = 0;
int n_dt_3_ac = 0;

void forward_packet(char* data, size_t len)
{
        int i;
        struct bus_node* node;
        struct bus_descriptor* dest_bus;
        struct bus_hdr* hdr = (struct bus_hdr*)data;
        struct bus_descriptor* bus;

        dest_bus = NULL;
        for(i = 0; i < n_busses && dest_bus == NULL; i++) {
                bus = &(busses[i]);
                for(node = bus->layout; node; node = node->next) {
                        if(node->devtype == hdr->dtype || node->addr == hdr->daddr) {
                                dest_bus = bus;
                                break;
                        }
                }
        }
        if(hdr->dtype == 3)
                n_dt_3++;
        if(dest_bus)
                n_dt_3_ac++;

        if(dest_bus)
                bus_write(dest_bus, data, len);

}


#endif /* BUS_NODETYPE == BUS_ROOT */
