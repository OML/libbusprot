#include "bus.h"
#include "bus_types.h"
#include "offsets.h"
#include "internal.h"

int bus_send_done(struct bus_descriptor* bus, bus_addr_t daddr)
{
        int len;
        struct bus_hdr* header;
        
        len = sizeof(struct bus_hdr);
        char buffer[len];

        header = get_bus_header(buffer);
        header->daddr = daddr;
        header->dtype = DT_NULL;
        header->stype = bus_node_type;
        header->opcode.op = BUSOP_DONE;
        header->saddr = addr;
        bus_write(bus, buffer, len);
        return 0;
}
