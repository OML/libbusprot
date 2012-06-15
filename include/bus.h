#ifndef _BUSPROT_H
#define _BUSPROT_H

#include <bus_types.h>
#include <bus_config.h>

#ifndef BUS_NODETYPE
#error "BUS_NODETYPE is not defined. Please define it as either BUS_ROOT or BUS_NODE."
#endif

#define BUS_ROOT 1
#define BUS_NODE 2


#define N_AVAILABLE_ADDRESSES 16

#define BUS_TIMEOUT 250
#define BUS_BUFFER_SIZE 32

typedef unsigned int size_t;

struct uart_ep_descriptor
{
        char data[BUS_BUFFER_SIZE];
        int pos;
        int len;
};

struct uart_descriptor
{
        volatile unsigned int* rxreg;
        volatile unsigned int* txreg;
        volatile unsigned int* stareg;

        struct uart_ep_descriptor rx_ep;
        struct uart_ep_descriptor tx_ep;
};

int uart_descriptor_bytes_available(struct uart_descriptor* desc);


struct bus_descriptor
{
        struct uart_descriptor uart;      
};

struct bus_node
{
        int devtype;
        struct bus_descriptor* bus;
};


#ifdef BUS_ROOT
extern size_t n_nodes;
extern struct bus_node nodes[N_AVAILABLE_ADDRESSES-1];
#endif // BUS_ROOT


extern struct bus_descriptor* busses;
extern size_t n_busses;

extern bus_addr_t addr;
extern bus_addr_t root_addr;
extern int bus_node_type;


extern void incoming_event(struct bus_descriptor* bus, char* data, size_t len);
extern unsigned long int rt_clock(void);
extern void uart_init(struct uart_descriptor* desc, size_t id);




void bus_init(size_t n_busses, int node_type);
void bus_do_work(void);

void bus_send_event(const char* data, size_t size);



void uart_has_byte_available(struct uart_descriptor* uart);

#endif /* busprot.h */
