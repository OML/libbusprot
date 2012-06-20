#include "bus.h"
#if BUS_NODETYPE == BUS_ROOT

#include "packets.h"

#include <stdlib.h>
#include <string.h>

struct bus_node nodes[N_AVAILABLE_ADDRESSES];
unsigned int n_nodes = 1;

void bus_init_single(struct bus_descriptor* bus)
{
        int rv;
        do {
                rv = bus_send_hello(bus, n_nodes++);
        } while(rv != 0 && n_nodes < N_AVAILABLE_ADDRESSES);

        if(n_nodes == N_AVAILABLE_ADDRESSES)
                __builtin_nop(); // Debugging
}

void bus_init(size_t _n_busses, int node_type)
{
        int i;
        struct bus_descriptor* bus;
        struct uart_descriptor* uart;

        memset(nodes, 0, sizeof(nodes));

        bus_node_type = node_type;
        n_busses = _n_busses;
        busses = (struct bus_descriptor*)malloc(sizeof(struct bus_descriptor) * n_busses);

        /* Wait 2 seconds for other devices to initialize */
        while(rt_clock() < 2000); 

        for(i = 0; i < n_busses; i++) {
                bus = &(busses[i]);
                uart = &(bus->uart);

                uart->rx_ep.pos = uart->rx_ep.len = 0;
                uart->tx_ep.pos = uart->tx_ep.len = 0;

                uart_init(uart, i);
                //bus_init_single(bus);
        }

        nodes[0].devtype = DT_IPC;
        nodes[0].bus = &busses[0];
        
        nodes[1].devtype = DT_DUAL_MOTOR_FRONT;
        nodes[1].bus = &busses[1];
        
        nodes[2].devtype = DT_DUAL_MOTOR_BACK;
        nodes[2].bus = &busses[2];

        n_nodes = 3;
        __builtin_nop();
}


#endif /* BUS_NODETYPE == BUS_ROOT */
