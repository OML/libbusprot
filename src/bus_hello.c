#include "bus.h"
#if BUS_NODETYPE == BUS_ROOT

#include "packets.h"
#include "offsets.h"
#include "internal.h"
#include <stdlib.h>

int bus_send_hello(struct bus_descriptor* bus, bus_addr_t new_addr)
{
        uint32_t start;
        
        struct bus_hdr* header;
        struct bus_hello* hello;
        volatile struct bus_hello_reply* hello_reply;
        struct bus_node* node;

        struct uart_descriptor* uart = &(bus->uart);


        int hello_reply_len = sizeof(struct bus_hdr) + sizeof(struct bus_hello_reply);
        char rx_buffer[hello_reply_len];

        
        int hello_request_len = sizeof(struct bus_hdr) + sizeof(struct bus_hello);
        char tx_buffer[hello_request_len];
        
        hello_reply = get_bus_hello_reply(rx_buffer);

        hello = get_bus_hello(tx_buffer);
        header = get_bus_header(tx_buffer);



        header->opcode.op = BUSOP_HELLO;
        header->saddr = 0;
        header->daddr = new_addr;
                
                         
        start = rt_clock();

        bus_write(bus, tx_buffer, hello_request_len);
        size_t bytes_available;
        while(rt_clock() - start < BUS_TIMEOUT) {
                bytes_available = uart_descriptor_bytes_available(uart);
                if(bytes_available > 0)
                        break;
        }
        
        if(bytes_available == 0)
                return 0; /* Timeout */

        bus_read(bus, rx_buffer, hello_reply_len);
        header = get_bus_header(rx_buffer);
        hello_reply = get_bus_hello_reply(rx_buffer);

        if(hello_reply->devtype == 2)
                __builtin_nop();

        if(header->opcode.op != BUSOP_HELLO)
                return 0;     

        node = &nodes[new_addr];
        node->devtype = hello_reply->devtype;
        node->bus = bus;


        __builtin_nop();

        return 1;
}

#endif /* BUS_NODETYPE == BUS_ROOT */
