#ifndef _BUSPROT_H
#define _BUSPROT_H

#include "bus_types.h"


#define BUS_NODETYPE BUS_ROOT


#ifndef BUS_NODETYPE
#error "BUS_NODETYPE is not defined. Please define it as either BUS_ROOT or BUS_NODE."
#endif

#define BUS_ROOT 1
#define BUS_NODE 2

#if BUS_N_UARTS > 2 && BUS_NODETYPE != BUS_ROOT
#error "Only ROOT nodes can have more than two bus endpoints"
#endif


#define BUS_N_BUSSES BUS_N_UARTS


#define BUS_TIMEOUT 250
#define BUS_BUFFER_SIZE 32

typedef unsigned int size_t;

struct bus_node
{
        struct bus_node* next;

        int devtype;
        uint16_t addr;
};

struct uart_ep_descriptor
{
        char data[BUS_BUFFER_SIZE];
        size_t pos;
        size_t len;
};

struct uart_descriptor
{
        volatile unsigned int* rxreg;
        volatile unsigned int* txreg;
        volatile unsigned int* txsta;

        struct uart_ep_descriptor rx_ep;
        struct uart_ep_descriptor tx_ep;
};

int uart_descriptor_bytes_available(struct uart_descriptor* desc);


struct bus_descriptor
{
        struct uart_descriptor uart;
        struct bus_node* layout;       
};
void bus_descriptor_add_node(struct bus_descriptor* desc, 
                                struct bus_node* node);


extern struct bus_descriptor* busses;
extern size_t n_busses;


extern bus_addr_t addr;
extern bus_addr_t root_addr;

extern unsigned long int rt_clock(void);



void uart_has_byte_available(struct uart_descriptor* uart);



void bus_init(size_t n_busses);
void bus_do_work(void);


#endif /* busprot.h */
