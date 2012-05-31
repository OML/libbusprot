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
        struct bus_hello_reply* hello_reply;
        struct bus_node* node;


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
        while(rt_clock() - start < BUS_TIMEOUT) {
                if(uart_descriptor_bytes_available(&(bus->uart)))
                        break;
        }
        
        if(uart_descriptor_bytes_available(&(bus->uart)) == 0)
                return 0; /* Timeout */

        bus_read(bus, rx_buffer, hello_reply_len);
                        
        node = (struct bus_node*)malloc(sizeof(struct bus_node));
        node->next = NULL;
        node->addr = new_addr;
        node->devtype = hello_reply->devtype;

        bus_descriptor_add_node(bus, node);

        return 1;
}

#endif /* BUS_NODETYPE == BUS_ROOT */
