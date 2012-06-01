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
        char* buffer;
        struct bus_hdr* hdr;
        struct bus_descriptor* src_bus;

        for(bus_id = 0; bus_id < n_busses; bus_id++) {
                src_bus = &busses[bus_id];

                struct bus_descriptor* bus0 = &(busses[0]);
                if(bus0->uart.rx_ep.len > 0)
                        __builtin_nop(); 

                if(uart_descriptor_bytes_available(&(src_bus->uart))) {
                        len = uart_descriptor_bytes_available(&(src_bus->uart));
                        buffer = (char*)malloc(len);
                        bus_read(src_bus, buffer, len);


                        hdr = get_bus_header(buffer);
                        if(hdr->daddr == 0) {
                                if(hdr->opcode.op == BUSOP_ACQUIRE_ADDRESS)
                                        bus_send_hello(src_bus, addresses);
                        }                   
                        else
                                forward_packet(buffer, len);  

                        free(buffer);
                }
        }
}

#endif /* BUS_NODETYPE == NODE_ROOT */
