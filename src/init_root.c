#include "bus.h"
#if BUS_NODETYPE == BUS_ROOT

#include "packets.h"

#include <stdlib.h>


unsigned int addresses = 1;

void bus_init_single(struct bus_descriptor* bus)
{
        bus->layout = NULL;
        while(bus_send_hello(bus, addresses++) != 0);
}

void bus_init(size_t _n_busses, int node_type)
{
        int i;
        struct bus_descriptor* bus;
        struct uart_descriptor* uart;
        struct bus_node* testnode;

        bus_node_type = node_type;
        n_busses = _n_busses;
        busses = (struct bus_descriptor*)malloc(sizeof(struct bus_descriptor) * n_busses);

        /* Wait 2 seconds for other devices to initialize */
        while(rt_clock() < 2000); 

        for(i = 0; i < n_busses; i++) {
                bus = &(busses[i]);
                bus->layout = NULL;
                uart = &(bus->uart);

                uart->rx_ep.pos = uart->rx_ep.len = 0;
                uart->tx_ep.pos = uart->tx_ep.len = 0;

                uart_init(uart, i);
                bus_init_single(bus);
        }
        __builtin_nop();
}


#endif /* BUS_NODETYPE == BUS_ROOT */
