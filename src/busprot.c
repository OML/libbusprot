#include "bus.h"

#include "internal.h"
#include "offsets.h"

#include <stdlib.h>
#include <string.h>

struct bus_descriptor* busses;
size_t n_busses;

bus_addr_t addr;
bus_addr_t root_addr;

void bus_init(size_t _n_busses)
{
        n_busses = _n_busses;
        int i;
        struct bus_descriptor* bus;
        busses = (struct bus_descriptor*)malloc(sizeof(struct bus_descriptor*) * n_busses);

        while(rt_clock() < 2000); // Wait 2 seconds for other devices to initialize

        for(i = 0; i < n_busses; i++) {
                bus = &(busses[i]);
                uart_init(&(bus->uart), i);
                bus_init_single(bus);
        }
}


void bus_descriptor_add_node(struct bus_descriptor* desc, 
                                struct bus_node* new_node)
{
        struct bus_node* iter;

        if(desc->layout == NULL)  {
                desc->layout = new_node;
                return;
        }
        
        iter = desc->layout;
        while(iter->next != NULL)
                iter = iter->next;
        iter->next = new_node;
}



int uart_descriptor_bytes_available(struct uart_descriptor* desc)
{
        if(desc->rx_ep.pos > 0 && desc->rx_ep.pos == desc->rx_ep.len)
                return desc->rx_ep.len;
        return 0;
}




void bus_write(struct bus_descriptor* bus, const char* data, size_t len)
{
        struct bus_hdr* hdr = get_bus_header(data);
        hdr->len = len;
        hdr->crc = 0;
        uart_write(&(bus->uart), data, len); 
}

void bus_read(struct bus_descriptor* desc, char* data, size_t len)
{
        uart_read(&(desc->uart), data, len);
}


size_t uart_read(struct uart_descriptor* uart, char* data, size_t len)
{
        len = min(uart->rx_ep.len, len);
        memcpy(data, uart->rx_ep.data, min(uart->rx_ep.len, len));
        return len;
}

size_t uart_write(struct uart_descriptor* uart, const char* data, size_t len)
{
	//memcpy((char*)ep->tx_buffer.data, data, length); // put the data in the buffer
#warning "Using non-interrupt driven UART writing methods"
	int i;
	for(i = 0; i < len; i++)
	{
                while(*(uart->txsta) & (1 << 9));
		*(uart->txreg) = data[i]; // write data to buffer
	}
        return len;
}


void uart_has_byte_available(struct uart_descriptor* uart)
{
        struct uart_ep_descriptor* ep = &(uart->rx_ep);
        ep->data[(ep->pos)++] = *(uart->rxreg);
        if(ep->pos == 2) {
                ep->len = get_bus_header(ep->data)->len;
        }
}
