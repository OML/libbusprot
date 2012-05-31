#include "bus.h"
#if BUS_NODETYPE == BUS_NODE

void bus_do_work(void)
{
        struct bus_hdr* hdr;
        char* buffer;
        size_t len;

        int i;
        for(i = 0; i < n_busses; i++) {
                len = uart_descriptor_bytes_available(&(busses[i].uart)); 
                if(len > 0) {
                        buffer = (char*)malloc(len);
                        bus_read(&(busses[i].uart), buffer, len);
                        
                        hdr = get_bus_header(buffer);
                        if(hdr->addr = addr) {
                                switch(hdr->opcode) {
                                }
                        } else {
                                bus_write(&busses[(i == 0) ? 1 : 0], buffer, len); 
                        }

                        free(buffer);
                }
        }
}

#endif /* BUS_NODETYPE == BUS_NODE */
