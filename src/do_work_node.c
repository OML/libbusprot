#include "bus.h"
#include "bus_types.h"
#if BUS_NODETYPE == BUS_NODE

#include <stdlib.h>

#include "internal.h"
#include "offsets.h"




// The descriptor on which we have to write to get to the root node
static struct bus_descriptor* root_descriptor;

static void process_hello(struct bus_descriptor* bus, char* request)
{
        root_descriptor = bus;

        struct bus_hdr* req_hdr = get_bus_header(request);
        struct bus_hello* req_hello = get_bus_hello(request);

        size_t resp_len = (size_t)get_bus_hello_reply(NULL) + sizeof(struct bus_hello_reply);
        char buffer[resp_len];
        struct bus_hdr* resp_hdr = get_bus_header(buffer);
        struct bus_hello_reply* resp_hello = get_bus_hello_reply(buffer);

		if(req_hdr->daddr <= addr)
		{
        	addr = req_hdr->daddr;
        	resp_hdr->opcode.op = BUSOP_HELLO;
        	resp_hdr->daddr = req_hdr->saddr;
        	resp_hdr->saddr = addr;
        	resp_hello->devtype = bus_node_type;
        	bus_write(bus, buffer, resp_len);
		}

}


void bus_do_work(void)
{
        struct bus_descriptor* bus;
		struct uart_descriptor* uart;
        struct bus_hdr* hdr;
        char buffer[32];
        size_t len;

        int i;
        for(i = 0; i < n_busses; i++) {
                bus = &busses[i];
				uart = &(bus->uart);
                len = uart_descriptor_bytes_available(uart); 
                if(len > 0) {
                        bus_read(bus, buffer, len);
                        
                        hdr = (struct bus_hdr*)buffer;

                        if(hdr->opcode.op == BUSOP_HELLO)
                          		process_hello(bus, buffer);
						else
						{
							__builtin_nop();

                        	if(hdr->daddr == addr) {
                              	  switch(hdr->opcode.op) {
                                	        case BUSOP_EVENT:
                                    	            incoming_event(bus, buffer, len);
                                        	        break;
                                      	 	 default:
                                        	        break;
                                	}
                        	}
						 
							else {
                            	    bus_write(&busses[(i == 0) ? 1 : 0], buffer, len); 
                        	}
						}
						__builtin_nop();
                        //free(buffer);
                }
        }
}

#endif /* BUS_NODETYPE == BUS_NODE */
