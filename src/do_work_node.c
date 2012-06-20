#include "bus.h"
#include "bus_types.h"
#if BUS_NODETYPE == BUS_NODE

#include <stdlib.h>

#include "internal.h"
#include "offsets.h"

// The descriptor on which we have to write to get to the root node
static struct bus_descriptor* root_descriptor = NULL;


void bus_send_event(const char* data, size_t len)
{
        int i;
        struct bus_descriptor* bus;
        if(root_descriptor == NULL) {
                for(i = 0; i < n_busses; i++) {
                        bus = &busses[i];
                        bus_write(bus, data, len);
                }
        } else {
                bus_write(root_descriptor, data, len);
        }
}

size_t n_adrs = 0;
unsigned long int prev_addr_session = 0;

static void process_hello(struct bus_descriptor* bus, char* request)
{
        root_descriptor = bus;

        volatile struct bus_hdr* req_hdr = get_bus_header(request);

        size_t resp_len = (size_t)get_bus_hello_reply(NULL) + sizeof(struct bus_hello_reply);
        volatile char buffer[resp_len];
        volatile struct bus_hdr* resp_hdr = get_bus_header(buffer);
        volatile struct bus_hello_reply* resp_hello = get_bus_hello_reply(buffer);

        n_adrs++;
	if(rt_clock() - prev_addr_session > 5000 || prev_addr_session == 0)
	{
                prev_addr_session = rt_clock();
                addr = req_hdr->daddr;
                resp_hdr->opcode.op = BUSOP_HELLO;
                resp_hdr->daddr = req_hdr->saddr;
                resp_hdr->saddr = addr;
                resp_hdr->stype = bus_node_type;
                resp_hello->devtype = bus_node_type;
                bus_write(bus, (char*)buffer, resp_len);
        }
        __builtin_nop();
}


void bus_do_work(void)
{
        struct bus_descriptor* bus;
		struct uart_descriptor* uart;
        struct bus_hdr* hdr;
        char buffer[BUS_BUFFER_SIZE];
        size_t len;

        int i;
        for(i = 0; i < n_busses; i++) {
                bus = &busses[i];
		uart = &(bus->uart);
                len = uart_descriptor_bytes_available(uart); 
                if(len > 0) {
                        bus_read(bus, buffer, len);
                        
                        hdr = get_bus_header(buffer);

                        if(hdr->opcode.op == BUSOP_HELLO)
				process_hello(bus, buffer);
                        else if(hdr->daddr == addr || hdr->dtype == bus_node_type) {
                                switch(hdr->opcode.op) {
                                        case BUSOP_EVENT:
                                                incoming_event(bus, buffer, len);
                                                break;
                                        default:
                                                break;
                                }
                        } else if(n_busses > 1) {
                                bus_write(&busses[(i == 0) ? 1 : 0], buffer, len); 
                        }

                }
        }
}

#endif /* BUS_NODETYPE == BUS_NODE */
