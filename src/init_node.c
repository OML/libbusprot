#include "bus.h"
#if BUS_NODETYPE == BUS_NODE

static void bus_init_single(struct bus_descriptor* bus)
{
}

void bus_init(size_t _n_busses)
{
        n_busses = _n_busses;
        int i;
        struct bus_descriptor* bus;
        busses = (struct bus_descriptor*)malloc(sizeof(struct bus_descriptor*) * n_busses);

        for(i = 0; i < n_busses; i++) {
                bus = &(busses[i]);
                uart_init(&(bus->uart), i);
                bus_init_single(bus);
        }
}


#endif /* BUS_NODETYPE == BUS_NODE */
