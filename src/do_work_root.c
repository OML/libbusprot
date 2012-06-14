#include "bus.h"
#if BUS_NODETYPE == BUS_ROOT

#include "packets.h"
#include "offsets.h"
#include "internal.h"
#include <stdlib.h>

extern bus_addr_t addresses;

void bus_do_work(void)
{
        int bus_id;
        int len;
        char buffer[BUS_BUFFER_SIZE];
        struct bus_hdr* hdr;
        struct bus_descriptor* src_bus;


        for(bus_id = 0; bus_id < n_busses; bus_id++) {
                src_bus = &busses[bus_id];
                len = uart_descriptor_bytes_available(&(src_bus->uart));

                if(len) {  
                        bus_read(src_bus, buffer, len);

                        hdr = (struct bus_hdr*)buffer;
                        if(hdr->daddr == 0 && hdr->dtype == DT_IPC) {
                                if(hdr->opcode.op == BUSOP_ACQUIRE_ADDRESS)
                                        bus_send_hello(src_bus, addresses);
                        }                   
                        else {
                                forward_packet(buffer, len);  
                                
                                bus_send_done(src_bus, hdr->saddr);       
                        }
                }
        }
}

#endif /* BUS_NODETYPE == NODE_ROOT */
