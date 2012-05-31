#include "busprot.h"
#if BUS_NODETYPE == BUS_ROOT

#include "packets.h"

#include <stdlib.h>


unsigned int addresses = 1;

void bus_init(struct bus_descriptor* bus)
{
        bus->layout = NULL;
        while(bus_send_hello(bus, addresses++) == 1);
}



#endif /* BUS_NODETYPE == BUS_ROOT */
