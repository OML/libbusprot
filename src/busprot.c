#include "bus.h"

#include "internal.h"
#include "offsets.h"

#include <stdlib.h>
#include <string.h>


struct bus_descriptor* busses;
size_t n_busses;

bus_addr_t addr = 0xFFFF;
bus_addr_t root_addr = 0;
int bus_node_type = 0;


int uart_descriptor_bytes_available(struct uart_descriptor* desc)
{
        if(desc->rx_ep.pos > 0 && desc->rx_ep.pos == desc->rx_ep.len)
                return desc->rx_ep.len;
        return 0;
}



void bus_write(struct bus_descriptor* bus, const char* data, size_t len)
{
        struct bus_hdr* hdr = (struct bus_hdr*)data;
        struct uart_descriptor* uart = &(bus->uart);
        hdr->len = len;
        hdr->crc = 0;
        
        uart_write(uart, data, len); 
}

void bus_read(struct bus_descriptor* desc, char* data, size_t len)
{
        uart_read(&(desc->uart), data, len);
}


size_t uart_read(struct uart_descriptor* uart, char* data, size_t len)
{
        len = min(uart->rx_ep.len, len);
        memcpy(data, uart->rx_ep.data, min(uart->rx_ep.len, len));
        uart->rx_ep.len = 0; 
        uart->rx_ep.pos = 0;
        return len;
}

size_t uart_write(struct uart_descriptor* uart, const char* data, size_t len)
{
        if(uart == NULL)
                return 0;

	memcpy((char*)uart->tx_ep.data, data, len); // put the data in the buffer 
#warning "Using non-interrupt driven UART writing methods"	
	int i;
	for(i = 0; i < len; i++) {
                while(!(*(uart->stareg) & (1 << 8)));
		*(uart->txreg) = data[i];
	}
        return len;
}

void uart_has_byte_available(struct uart_descriptor* uart)
{
        struct uart_ep_descriptor* ep = &(uart->rx_ep);
        if(ep->pos == ep->len && ep->pos != 0)
                ep->pos = ep->len = 0;
        ep->data[(ep->pos)++] = *(uart->rxreg);
        if(ep->pos == 2) {
                ep->len = get_bus_header(ep->data)->len;
        }
}
